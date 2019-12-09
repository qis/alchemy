#include "alchemy.hpp"

namespace alchemy {

struct Effect {
  std::string name;
  float cost = 0.0f;

  Effect() noexcept = default;
  Effect(RE::Effect* effect, float power) noexcept {
    name = effect->baseEffect->GetFullName();
    auto flags = effect->baseEffect->data.flags;
    auto baseCost = effect->baseEffect->data.baseCost;
    auto magnitude = effect->effectItem.magnitude;
    if (static_cast<UInt32>(flags) & static_cast<UInt32>(RE::EffectSetting::Data::Flag::kNoMagnitude) || magnitude < 0.0f) {
      magnitude = 1.0f;
    } else if (static_cast<UInt32>(flags) & static_cast<UInt32>(RE::EffectSetting::Data::Flag::kPowerAffectsMagnitude)) {
      magnitude = round(std::pow(magnitude * power, 1.1f));
    }
    auto duration = effect->effectItem.duration;
    if (static_cast<UInt32>(flags) & static_cast<UInt32>(RE::EffectSetting::Data::Flag::kNoDuration) || duration < 0.0f) {
      duration = 1.0f;
    } else if (static_cast<UInt32>(flags) & static_cast<UInt32>(RE::EffectSetting::Data::Flag::kPowerAffectsDuration)) {
      duration = round(std::pow(duration * power / 10.0f, 1.1f));
    }
    cost = floor(baseCost * magnitude * duration);
  }
};

struct Ingredient {
  UInt32 id = 0;
  SInt32 count = 0;
  std::string name;
  std::array<Effect, 4> effects;

  Ingredient() noexcept = default;
  Ingredient(UInt32 id, RE::TESBoundObject* object, SInt32 count, float power) noexcept : id(id), count(count) {
    const auto ingredient = object->As<RE::IngredientItem*>();
    name = ingredient->GetName();
    for (std::size_t i = 0; i < 4; i++) {
      effects[i] = { ingredient->effects[i], power };
    }
    std::sort(effects.begin(), effects.end(), [](const auto& lhs, const auto& rhs) noexcept {
      return lhs.name < rhs.name;
    });
  }
};

std::vector<Ingredient> GetIngredients(RE::PlayerCharacter* player) {
  const auto settings = RE::GameSettingCollection::GetSingleton();
  auto ingredientMult = settings->GetSetting("fAlchemyIngredientInitMult")->GetFloat();
  auto skillFactor = settings->GetSetting("fAlchemySkillFactor")->GetFloat();
  auto alchemySkill = player->GetActorValueCurrent(RE::ActorValue::kAlchemy);
  auto fortifyAlchemy = player->GetActorValueCurrent(RE::ActorValue::kAlchemyModifier);
  auto alchemistPerk = 0.0f;
  for (const auto& e : player->perks) {
    if (e->GetFullName() == std::string_view("Alchemy Mastery")) {
      alchemistPerk += 20.0f;
    }
  }
  // clang-format off
  const auto power =
    ingredientMult *
    (1.0f + (skillFactor - 1.0f) * alchemySkill / 100.0f) *
    (1.0f + fortifyAlchemy / 100.0f) *
    (1.0f + alchemistPerk / 100.0f);
  // clang-format on
  std::vector<Ingredient> ingredients;
  if (const auto changes = player->GetInventoryChanges(); changes && changes->entryList) {
    for (const auto& entry : *changes->entryList) {
      if (entry->type->GetFormType() == RE::FormType::Ingredient) {
        const auto id = entry->type->formID;
        const auto it = std::find_if(ingredients.begin(), ingredients.end(), [&](const auto& entry) noexcept {
          return entry.id == id;
        });
        if (it != ingredients.end()) {
          it->count += entry->countDelta;
        } else {
          ingredients.emplace_back(id, entry->type, entry->countDelta, power);
        }
      }
    }
  }
  player->GetContainer()->ForEach([&](RE::TESContainer::Entry* entry) {
    if (entry->form->GetFormType() == RE::FormType::Ingredient) {
      const auto id = entry->form->formID;
      const auto it = std::find_if(ingredients.begin(), ingredients.end(), [&](const auto& entry) noexcept {
        return entry.id == id;
      });
      if (it != ingredients.end()) {
        it->count += entry->count;
      } else {
        ingredients.emplace_back(id, entry->form, entry->count, power);
      }
    }
    return true;
  });
  std::sort(ingredients.begin(), ingredients.end(), [](const auto& lhs, const auto& rhs) noexcept {
    return lhs.name < rhs.name;
  });
  return ingredients;
}

struct Match {
  std::string name;
  float cost = 0.0f;
};

float GetCost(const Ingredient& i0, const Ingredient& i1) noexcept {
  std::size_t count = 0;
  std::array<Match, 4> matches;
  const auto update = [&](const Effect& e0, const Effect& e1) noexcept {
    if (e0.name != e1.name) {
      return;
    }
    const auto cost = std::max(e0.cost, e1.cost);
    for (std::size_t i = 0; i < count; i++) {
      if (matches[i].name == e0.name) {
        matches[i].cost = std::max(matches[i].cost, cost);
        return;
      }
    }
    matches[count++] = { e0.name, cost };
  };
  for (const auto& e0 : i0.effects) {
    for (const auto& e1 : i1.effects) {
      update(e0, e1);
    }
  }
  auto cost = 0.0f;
  for (std::size_t i = 0; i < count; i++) {
    cost += matches[i].cost;
  }
  return cost;
}

float GetCost(const Ingredient& i0, const Ingredient& i1, const Ingredient& i2) noexcept {
  std::size_t count = 0;
  std::array<Match, 6> matches;
  const auto update = [&](const Effect& e0, const Effect& e1) noexcept {
    if (e0.name != e1.name) {
      return;
    }
    const auto cost = std::max(e0.cost, e1.cost);
    for (std::size_t i = 0; i < count; i++) {
      if (matches[i].name == e0.name) {
        matches[i].cost = std::max(matches[i].cost, cost);
        return;
      }
    }
    matches[count++] = { e0.name, cost };
  };
  for (const auto& e0 : i0.effects) {
    for (const auto& e1 : i1.effects) {
      update(e0, e1);
    }
  }
  for (const auto& e0 : i0.effects) {
    for (const auto& e2 : i2.effects) {
      update(e0, e2);
    }
  }
  for (const auto& e1 : i1.effects) {
    for (const auto& e2 : i2.effects) {
      update(e1, e2);
    }
  }
  auto cost = 0.0f;
  for (std::size_t i = 0; i < count; i++) {
    cost += matches[i].cost;
  }
  return cost;
}

std::string_view GetUniqueName(const std::vector<Ingredient>& ingredients, const Ingredient& ingredient) noexcept {
  const auto beg = ingredients.begin();
  const auto end = ingredients.end();
  for (auto pos = ingredient.name.find(' '); pos != std::string_view::npos; pos = ingredient.name.find(pos + 1, ' ')) {
    std::string_view name{ ingredient.name.data(), pos };
    for (auto it = beg; it < end; ++it) {
      if (it->name == ingredient.name) {
        continue;
      }
      if (it->name.compare(0, pos, name) == 0) {
        goto retry;
      }
    }
    return name;
  retry:
    continue;
  }
  return ingredient.name;
}

void OnAlchemy(RE::PlayerCharacter* player) {
  const auto ingredients = GetIngredients(player);

  std::mutex mutex;
  float maxValue = 0.0f;
  std::array<Ingredient, 3> maxPotion;

  const std::size_t init = 0;
  const std::size_t size = ingredients.size();
  tbb::parallel_for(init, size > 0 ? size - 1 : 0, [&](std::size_t i0) noexcept {
    float curValue = 0.0f;
    std::array<Ingredient, 3> curPotion;
    for (std::size_t i1 = i0 + 1; i1 < size; i1++) {
      if (const auto value = GetCost(ingredients[i0], ingredients[i1]); value > curValue + 1.0f) {
        curValue = value;
        curPotion[0] = ingredients[i0];
        curPotion[1] = ingredients[i1];
        curPotion[2] = {};
      }
      for (std::size_t i2 = i1 + 1; i2 < size; i2++) {
        if (const auto value = GetCost(ingredients[i0], ingredients[i1], ingredients[i2]); value > curValue + 1.0f) {
          curValue = value;
          curPotion[0] = ingredients[i0];
          curPotion[1] = ingredients[i1];
          curPotion[2] = ingredients[i2];
        }
      }
    }
    std::lock_guard lock(mutex);
    if (curValue > maxValue + 1.0f) {
      maxValue = curValue;
      maxPotion = curPotion;
    }
  });

  const auto console = RE::ConsoleManager::GetSingleton();

  if (maxValue > 1.0f) {
    std::sort(maxPotion.begin(), maxPotion.end(), [](const auto& lhs, const auto& rhs) noexcept {
      return lhs.name < rhs.name;
    });
    fmt::memory_buffer buffer;
    for (const auto& e : maxPotion) {
      if (e.id) {
        if (buffer.size()) {
          fmt::format_to(buffer, " | ");
        }
        fmt::format_to(buffer, "{}", GetUniqueName(ingredients, e));
      }
    }
#if ALCHEMY_DEBUG
    fmt::format_to(buffer, " ({})", maxValue);
#endif
    buffer.push_back('\0');
    RE::DebugNotification(buffer.data());
    console->Print(buffer.data());
  }
}

class InputEventHandler : public RE::BSTEventSink<RE::InputEvent*> {
public:
  RE::EventResult ReceiveEvent(RE::InputEvent** event_ptr, RE::BSTEventSource<RE::InputEvent*>* source) override {
    if (!event_ptr || !*event_ptr) {
      return RE::EventResult::kContinue;
    }
    const auto event = *event_ptr;
    if (event->eventType == RE::InputEvent::EventType::kButton && event->deviceType == RE::DeviceType::kKeyboard) {
      const auto button = static_cast<RE::ButtonEvent*>(event);
      if (button->keyMask == 21 && button->IsDown() && RE::MenuManager::GetSingleton()->IsMenuOpen("Crafting Menu")) {
        if (const auto player = RE::PlayerCharacter::GetSingleton()) {
          try {
            OnAlchemy(player);
          }
          catch (const std::exception& e) {
            LOG_FATALERROR("alchemy exception: %s", e.what());
          }
          catch (...) {
            LOG_FATALERROR("alchemy exception");
          }
        }
        return RE::EventResult::kAbort;
      }
    }
    return RE::EventResult::kContinue;
  }

  static RE::BSTEventSink<RE::InputEvent*>* GetSingleton() {
    static InputEventHandler singleton;
    return &singleton;
  }

protected:
  InputEventHandler() = default;
  InputEventHandler(InputEventHandler&&) = delete;
  InputEventHandler(const InputEventHandler&) = delete;
  InputEventHandler& operator=(const InputEventHandler&) = delete;
  InputEventHandler& operator=(InputEventHandler&&) = delete;
  virtual ~InputEventHandler() = default;
};

void MessageHandler(SKSE::MessagingInterface::Message* message) {
  switch (message->type) {
  case SKSE::MessagingInterface::kDataLoaded:
    RE::InputManager::GetSingleton()->AddEventSink(InputEventHandler::GetSingleton());
    break;
  default:
    break;
  }
}

void Load() {
  LOGI("plugin loaded");
  if (!SKSE::GetMessagingInterface()->RegisterListener("SKSE", MessageHandler)) {
    throw std::runtime_error("could not register message handler");
  }
  LOGI("registered message handler");
}

}  // namespace alchemy
