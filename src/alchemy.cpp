#include "alchemy.hpp"

namespace alchemy {

struct Effect {
  RE::EffectSetting::Data::Archetype archetype = RE::EffectSetting::Data::Archetype::kScript;
  RE::EffectSetting::Data::Flag flags = RE::EffectSetting::Data::Flag::kNone;
  float baseCost = 0.0f;
  UInt32 duration = 0;
  float magnitude = 0.0f;

  constexpr Effect() noexcept = default;
  constexpr Effect(RE::Effect* effect) noexcept {
    archetype = effect->baseEffect->data.archetype;
    flags = effect->baseEffect->data.flags;
    baseCost = effect->baseEffect->data.baseCost;
    duration = effect->effectItem.duration;
    magnitude = effect->effectItem.magnitude;
  }
};

struct Ingredient {
  UInt32 id = 0;
  SInt32 count = 0;
  std::string name;
  std::array<Effect, 4> effects;

  Ingredient() noexcept = default;
  Ingredient(UInt32 id, RE::TESBoundObject* object, SInt32 count) noexcept : id(id), count(count) {
    const auto ingredient = object->As<RE::IngredientItem*>();
    name = ingredient->GetName();
    if (ingredient->effects.size() >= 4) {
      for (std::size_t i = 0; i < 4; i++) {
        effects[i] = ingredient->effects[i];
      }
    }
  }
};

std::vector<Ingredient> GetIngredients(RE::PlayerCharacter* player) {
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
          ingredients.emplace_back(id, entry->type, entry->countDelta);
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
        ingredients.emplace_back(id, entry->form, entry->count);
      }
    }
    return true;
  });
  std::sort(ingredients.begin(), ingredients.end(), [](const auto& lhs, const auto& rhs) noexcept {
    return lhs.name < rhs.name;
  });
  return ingredients;
}

struct Alchemy {
  float powerFactor = 1.0f;

  Alchemy(RE::PlayerCharacter* player) {
    const auto settings = RE::GameSettingCollection::GetSingleton();
    const auto ingredientMult = settings->GetSetting("fAlchemyIngredientInitMult")->GetFloat();
    const auto skillFactor = settings->GetSetting("fAlchemySkillFactor")->GetFloat();

    const auto alchemySkill = player->GetActorValueCurrent(RE::ActorValue::kAlchemy);
    const auto fortifyAlchemy = player->GetActorValueCurrent(RE::ActorValue::kAlchemyModifier);

    auto alchemistPerk = 0.0f;
    //for (const auto& e : player->perks) {
    //  if (e->GetFullName() == std::string_view("Alchemy Mastery")) {
    //    alchemistPerk += 20.0f;
    //  }
    //}

    powerFactor = ingredientMult * (1.0f + (skillFactor - 1.0f) * alchemySkill / 100.0f) *
      (1.0f + fortifyAlchemy / 100.0f) * (1.0f + alchemistPerk / 100.0f);
  }

  float GetValue(const Effect& e) const noexcept {
    // Magnitude
    auto magnitude = e.magnitude;
    if (static_cast<UInt32>(e.flags) & static_cast<UInt32>(RE::EffectSetting::Data::Flag::kNoMagnitude)) {
      magnitude = 0.0f;
    }
    auto magnitudeFactor = 1.0f;
    if (static_cast<UInt32>(e.flags) & static_cast<UInt32>(RE::EffectSetting::Data::Flag::kPowerAffectsMagnitude)) {
      magnitudeFactor = powerFactor;
    }
    magnitude = round(magnitude * magnitudeFactor);

    // Duration
    auto duration = e.duration;
    if (static_cast<UInt32>(e.flags) & static_cast<UInt32>(RE::EffectSetting::Data::Flag::kNoDuration) || duration < 0.0f) {
      duration = 0.0f;
    }
    auto durationFactor = 1.0f;
    if (static_cast<UInt32>(e.flags) & static_cast<UInt32>(RE::EffectSetting::Data::Flag::kPowerAffectsDuration)) {
      durationFactor = powerFactor;
    }
    duration = round(duration * durationFactor);

    // Magnitude Factor
    magnitudeFactor = 1.0f;
    if (magnitude > 0.0f) {
      magnitudeFactor = magnitude;
    }

    // Duration Factor
    durationFactor = 1.0f;
    if (duration > 0.0f) {
      durationFactor = duration / 10.0f;
    }

    // Cost
    return floor(e.baseCost * std::pow(magnitudeFactor * durationFactor, 1.1f));
  }

  struct Match {
    RE::EffectSetting::Data::Archetype archetype = RE::EffectSetting::Data::Archetype::kScript;
    float value = 0.0f;
  };

  float GetValue(const Ingredient& i0, const Ingredient& i1) const noexcept {
    std::size_t count = 0;
    std::array<Match, 4> matches;

    const auto update = [&](const Effect& one, const Effect& two) noexcept {
      if (one.archetype != two.archetype) {
        return;
      }
      for (std::size_t i = 0; i < count; i++) {
        if (matches[i].archetype == one.archetype) {
          matches[i].value = std::max(std::max(matches[i].value, GetValue(one)), GetValue(two));
          return;
        }
      }
      matches[count++] = { one.archetype, std::max(GetValue(one), GetValue(two)) };
    };

    for (const auto& e0 : i0.effects) {
      for (const auto& e1 : i1.effects) {
        update(e0, e1);
      }
    }

    float value = 0.0f;
    for (std::size_t i = 0; i < count; i++) {
      value += matches[i].value;
    }
    return value;
  }

  float GetValue(const Ingredient& i0, const Ingredient& i1, const Ingredient& i2) const noexcept {
    std::size_t count = 0;
    std::array<Match, 6> matches;

    const auto update = [&](const Effect& one, const Effect& two) noexcept {
      if (one.archetype != two.archetype) {
        return;
      }
      for (std::size_t i = 0; i < count; i++) {
        if (matches[i].archetype == one.archetype) {
          matches[i].value = std::max(std::max(matches[i].value, GetValue(one)), GetValue(two));
          return;
        }
      }
      matches[count++] = { one.archetype, std::max(GetValue(one), GetValue(two)) };
    };

    for (std::size_t ei0 = 0; ei0 < i0.effects.size(); ei0++) {
      const auto& e0 = i0.effects[ei0];
      for (std::size_t ei1 = 0; ei1 < i1.effects.size(); ei1++) {
        const auto& e1 = i1.effects[ei1];
        for (std::size_t ei2 = 0; ei2 < i2.effects.size(); ei2++) {
          const auto& e2 = i2.effects[ei2];
          update(e0, e1);
          update(e0, e2);
          update(e1, e2);
        }
      }
    }

    float value = 0.0f;
    for (std::size_t i = 0; i < count; i++) {
      value += matches[i].value;
    }
    return value;
  }
};

void OnAlchemy(RE::PlayerCharacter* player) {
  const auto alchemy = Alchemy{ player };
  const auto ingredients = GetIngredients(player);

  std::mutex mutex;
  float maxValue = 0.0f;
  std::array<Ingredient, 3> maxPotion;

  const std::size_t init = 0;
  const std::size_t size = ingredients.size();
  tbb::parallel_for(init, size > 0 ? size - 1: 0, [&](std::size_t i0) noexcept {
    float curValue = 0.0f;
    std::array<Ingredient, 3> curPotion;
    for (std::size_t i1 = i0 + 1; i1 < size; i1++) {
      if (const auto value = alchemy.GetValue(ingredients[i0], ingredients[i1]); value > curValue) {
        curValue = value + 0.0f;
        curPotion[0] = ingredients[i0];
        curPotion[1] = ingredients[i1];
        curPotion[2] = {};
      }
      for (std::size_t i2 = 0; i2 < size; i2++) {
        if (i2 == i0 || i2 == i1) {
          continue;
        }
        if (const auto value = alchemy.GetValue(ingredients[i0], ingredients[i1], ingredients[i2]); value > curValue) {
          curValue = value + 0.0f;
          curPotion[0] = ingredients[i0];
          curPotion[1] = ingredients[i1];
          curPotion[2] = ingredients[i2];
        }
      }
    }
    std::lock_guard lock(mutex);
    if (curValue > maxValue) {
      maxValue = curValue;
      maxPotion = curPotion;
    }
  });

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
        fmt::format_to(buffer, "{}", e.name);
      }
    }
    const auto notification = std::string{ buffer.data(), buffer.size() };
    RE::DebugNotification(notification.data());
    if (const auto console = RE::ConsoleManager::GetSingleton()) {
      console->Print(notification.data());
    }
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
