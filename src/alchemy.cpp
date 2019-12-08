#include "alchemy.hpp"

namespace alchemy {

struct Effect {
  RE::EffectSetting::Data::Archetype archetype = RE::EffectSetting::Data::Archetype::kScript;
  RE::EffectSetting::Data::Flag flags = RE::EffectSetting::Data::Flag::kNone;
  float baseCost = 0.0f;
  UInt32 duration = 0;
  float magnitude = 0.0f;
  std::string name;

  Effect() noexcept = default;
  Effect(RE::Effect* effect) noexcept {
    archetype = effect->baseEffect->data.archetype;
    flags = effect->baseEffect->data.flags;
    baseCost = effect->baseEffect->data.baseCost;
    duration = effect->effectItem.duration;
    magnitude = effect->effectItem.magnitude;
    name = effect->baseEffect->GetFullName();
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
      std::sort(effects.begin(), effects.end(), [](const auto& lhs, const auto& rhs) noexcept {
        return static_cast<UInt32>(lhs.archetype) < static_cast<UInt32>(rhs.archetype);
      });
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
#if ALCHEMY_PRECISE
  std::sort(ingredients.begin(), ingredients.end(), [](const auto& lhs, const auto& rhs) noexcept {
    return lhs.name < rhs.name;
  });
#endif
  return ingredients;
}

struct Alchemy {
#if ALCHEMY_PRECISE
  float powerFactor = 1.0f;
#endif

  Alchemy(RE::PlayerCharacter* player) {
#if ALCHEMY_PRECISE
    const auto settings = RE::GameSettingCollection::GetSingleton();

    auto ingredientMult = settings->GetSetting("fAlchemyIngredientInitMult")->GetFloat();
    if (ingredientMult <= 0.001) {
      ingredientMult = 1.0f;
    }

    auto skillFactor = settings->GetSetting("fAlchemySkillFactor")->GetFloat();
    if (skillFactor < 1.0f) {
      skillFactor = 1.0f;
    }

    auto alchemySkill = player->GetActorValueCurrent(RE::ActorValue::kAlchemy);
    if (alchemySkill < 0.0f) {
      alchemySkill = 0.0f;
    }

    auto fortifyAlchemy = player->GetActorValueCurrent(RE::ActorValue::kAlchemyModifier);
    if (fortifyAlchemy < 0.0f) {
      fortifyAlchemy = 0.0f;
    }

    auto alchemistPerk = 0.0f;
    //for (const auto& e : player->perks) {
    //  if (e->GetFullName() == std::string_view("Alchemy Mastery")) {
    //    alchemistPerk += 20.0f;
    //  }
    //}

    // clang-format off
    powerFactor =
      ingredientMult *
      (1.0f + (skillFactor - 1.0f) * alchemySkill / 100.0f) *
      (1.0f + fortifyAlchemy / 100.0f) *
      (1.0f + alchemistPerk / 100.0f);
    // clang-format on
#endif
  }

  float GetValue(const Effect& e) const noexcept {
#if ALCHEMY_PRECISE
    // Magnitude
    auto magnitude = e.magnitude;
    if (static_cast<UInt32>(e.flags) & static_cast<UInt32>(RE::EffectSetting::Data::Flag::kNoMagnitude) || magnitude < 0.0f) {
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
    return floor(std::pow(e.baseCost * (magnitudeFactor * durationFactor), 1.1f));
#else
    auto multiplier = 1.0f;
    if (!(static_cast<UInt32>(e.flags) & static_cast<UInt32>(RE::EffectSetting::Data::Flag::kNoMagnitude)) && e.magnitude > 0.0f) {
      multiplier *= e.magnitude;
    }
    if (!(static_cast<UInt32>(e.flags) & static_cast<UInt32>(RE::EffectSetting::Data::Flag::kNoDuration)) && e.duration > 0.0f) {
      multiplier *= e.duration;
    }
    return floor(e.baseCost * multiplier);
#endif
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
      const auto value = std::max(GetValue(one), GetValue(two));
      for (std::size_t i = 0; i < count; i++) {
        if (matches[i].archetype == one.archetype) {
          matches[i].value = std::max(matches[i].value, value);
          return;
        }
      }
      matches[count++] = { one.archetype, value };
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
    std::array<bool, 3> used = { false, false, false };

    const auto update = [&](const Effect& one, const Effect& two) noexcept {
      if (one.archetype != two.archetype) {
        return false;
      }
      const auto value = std::max(GetValue(one), GetValue(two));
      for (std::size_t i = 0; i < count; i++) {
        if (matches[i].archetype == one.archetype) {
          matches[i].value = std::max(matches[i].value, value);
          return false;
        }
      }
      matches[count++] = { one.archetype, value };
      return true;
    };

    for (const auto& e0 : i0.effects) {
      for (const auto& e1 : i1.effects) {
        for (const auto& e2 : i2.effects) {
          if (update(e0, e1)) {
            used[0] = used[1] = true;
          }
          if (update(e0, e2)) {
            used[0] = used[2] = true;
          }
          if (update(e1, e2)) {
            used[1] = used[2] = true;
          }
        }
      }
    }

    for (const auto e : used) {
      if (!e) {
        return 0.0f;
      }
    }

    float value = 0.0f;
    for (std::size_t i = 0; i < count; i++) {
      value += matches[i].value;
    }
    return value;
  }
};

std::string_view GetUniqueName(const std::vector<Ingredient>& ingredients, const Ingredient& ingredient) noexcept {
  std::string_view name = ingredient.name;
  const auto beg = ingredients.begin();
  const auto end = ingredients.end();
  for (auto pos = name.find(' '); pos != std::string_view::npos; pos = name.find(pos + 1, ' ')) {
    for (auto it = beg; it < end; ++it) {
      if (it->name == ingredient.name) {
        continue;
      }
      if (it->name.compare(0, pos, name) == 0) {
        goto retry;
      }
    }
    name = { ingredient.name.data(), pos };
    break;
  retry:
    continue;
  }
  return name;
}

void OnAlchemy(RE::PlayerCharacter* player) {
  const auto alchemy = Alchemy{ player };
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
      if (const auto value = alchemy.GetValue(ingredients[i0], ingredients[i1]); value > curValue + 1.0f) {
        curValue = value;
        curPotion[0] = ingredients[i0];
        curPotion[1] = ingredients[i1];
        curPotion[2] = {};
      }
      for (std::size_t i2 = i1 + 1; i2 < size; i2++) {
        if (const auto value = alchemy.GetValue(ingredients[i0], ingredients[i1], ingredients[i2]); value > curValue + 1.0f) {
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

    fmt::memory_buffer bufferDebug;

    for (const auto& e : maxPotion) {
      if (e.id) {
        if (buffer.size()) {
          fmt::format_to(buffer, " | ");
        }
        fmt::format_to(buffer, "{}", GetUniqueName(ingredients, e));

        // clang-format off
        fmt::format_to(bufferDebug, "{}: {} ({}) | {} ({}) | {} ({}) | {} ({})\n", e.name,
          e.effects[0].name, alchemy.GetValue(e.effects[0]),
          e.effects[1].name, alchemy.GetValue(e.effects[1]),
          e.effects[2].name, alchemy.GetValue(e.effects[2]),
          e.effects[3].name, alchemy.GetValue(e.effects[3]));
        // clang-format on
      }
    }
    buffer.push_back('\0');
    RE::DebugNotification(buffer.data());
    console->Print(buffer.data());

    bufferDebug.push_back('\0');
    console->Print(bufferDebug.data());
  }

#if ALCHEMY_DEBUG
  const auto beg = ingredients.begin();
  const auto end = ingredients.end();
  const auto chaurus_eggs = std::find_if(beg, end, [](const auto& ingredient) noexcept {
    return ingredient.name == "Chaurus Eggs";
  });
  if (chaurus_eggs == end) {
    console->Print("Could not find ingredient: Chaurus Eggs");
    return;
  }
  const auto spider_egg = std::find_if(beg, end, [](const auto& ingredient) noexcept {
    return ingredient.name == "Spider Egg";
  });
  if (spider_egg == end) {
    console->Print("Could not find ingredient: Spider Egg");
    return;
  }
  const auto vampire_dust = std::find_if(beg, end, [](const auto& ingredient) noexcept {
    return ingredient.name == "Vampire Dust";
  });
  if (vampire_dust == end) {
    console->Print("Could not find ingredient: Vampire Dust");
    return;
  }
  const auto glow_dust = std::find_if(beg, end, [](const auto& ingredient) noexcept {
    return ingredient.name == "Glow Dust";
  });
  if (glow_dust == end) {
    console->Print("Could not find ingredient: Glow Dust");
    return;
  }
  const auto imp_stool = std::find_if(beg, end, [](const auto& ingredient) noexcept {
    return ingredient.name == "Imp Stool";
  });
  if (imp_stool == end) {
    console->Print("Could not find ingredient: Imp Stool");
    return;
  }
  const auto swamp_fungal_pod = std::find_if(beg, end, [](const auto& ingredient) noexcept {
    return ingredient.name == "Swamp Fungal Pod";
  });
  if (swamp_fungal_pod == end) {
    console->Print("Could not find ingredient: Swamp Fungal Pod");
    return;
  }
  const auto one = alchemy.GetValue(*chaurus_eggs, *spider_egg, *vampire_dust);
  const auto one_expected = 190.0f;
  const auto two = alchemy.GetValue(*glow_dust, *imp_stool, *swamp_fungal_pod);
  const auto two_expected = 257.0f;
  // clang-format off
  console->Print(fmt::format("{}/{} = {} | {}/{} = {}",
    static_cast<int>(one), static_cast<int>(one_expected), one / one_expected,
    static_cast<int>(two), static_cast<int>(two_expected), two / two_expected
  ).data());
  // clang-format on
#endif
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
