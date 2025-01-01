#include "extractor.h"

#include <algorithm>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <set>
#include <string>
#include <regex>
#include <sstream>

#include <json.hpp>
using json = nlohmann::json;

#define ROM_START 0x8000000
#define DEXSANITY_FLAGS_START 0x5000
#define FAMESANITY_FLAGS_START 0x6000

std::map<int, std::string> GAME_VERSION_MAP = {
    {0, "firered"},
    {1, "leafgreen"},
    {2, "firered"},
    {3, "leafgreen"}
};

std::map<int, std::string> GAME_REVISION_MAP = {
    {0, "firered"},
    {1, "leafgreen"},
    {2, "firered_rev1"},
    {3, "leafgreen_rev1"}
};

std::map<int, std::string> GAME_NAME_MAP = {
    {0, "FireRed"},
    {1, "LeafGreen"},
    {2, "FireRed Rev 1"},
    {3, "LeafGreen Rev 1"}
};

std::map<int, std::string> TRADE_POKEMON_MAP = {
    {0, "TRADE_POKEMON_MR_MIME"},
    {1, "TRADE_POKEMON_JYNX"},
    {2, "TRADE_POKEMON_NIDORAN"},
    {3, "TRADE_POKEMON_FARFETCHD"},
    {4, "TRADE_POKEMON_NIDORINOA"},
    {5, "TRADE_POKEMON_LICKITUNG"},
    {6, "TRADE_POKEMON_ELECTRODE"},
    {7, "TRADE_POKEMON_TANGELA"},
    {8, "TRADE_POKEMON_SEEL"}
};

const std::set<std::string> IGNORABLE_TRAINER_REWARDS = {
    "TRAINER_AQUA_ADMIN_MATT",
    "TRAINER_AQUA_ADMIN_SHELLY",
    "TRAINER_AQUA_GRUNT_F",
    "TRAINER_AQUA_GRUNT_M",
    "TRAINER_AQUA_LEADER",
    "TRAINER_BATTLE_GIRL",
    "TRAINER_BEAUTY_GRACE_2",
    "TRAINER_BEAUTY_LAUREN",
    "TRAINER_BEAUTY_SHEILA_2",
    "TRAINER_BIKER_1",
    "TRAINER_BIKER_2",
    "TRAINER_BIKER_JAREN_2",
    "TRAINER_BIKER_JAXON_2",
    "TRAINER_BIKER_LUKAS_2",
    "TRAINER_BIKER_RUBEN_2",
    "TRAINER_BIRD_KEEPER_BENNY_2",
    "TRAINER_BIRD_KEEPER_BENNY_3",
    "TRAINER_BIRD_KEEPER_CHAZ_2",
    "TRAINER_BIRD_KEEPER_CHESTER_2",
    "TRAINER_BIRD_KEEPER_CHESTER_3",
    "TRAINER_BIRD_KEEPER_HAROLD_2",
    "TRAINER_BIRD_KEEPER_JACOB_2",
    "TRAINER_BIRD_KEEPER_JACOB_3",
    "TRAINER_BIRD_KEEPER_KEITH",
    "TRAINER_BIRD_KEEPER_MARLON_2",
    "TRAINER_BIRD_KEEPER_MARLON_3",
    "TRAINER_BIRD_KEEPER_MILO_2",
    "TRAINER_BIRD_KEEPER_REED",
    "TRAINER_BIRD_KEEPER_ROBERT_2",
    "TRAINER_BIRD_KEEPER_ROBERT_3",
    "TRAINER_BLACK_BELT_HUGH_2",
    "TRAINER_BLACK_BELT_HUGH_3",
    "TRAINER_BLACK_BELT_SHEA_2",
    "TRAINER_BLACK_BELT_SHEA_3",
    "TRAINER_BOARDER_F",
    "TRAINER_BOARDER_M",
    "TRAINER_BRENDAN_2",
    "TRAINER_BRENDAN_3",
    "TRAINER_BRENDAN",
    "TRAINER_BUG_CATCHER_2",
    "TRAINER_BUG_CATCHER_3",
    "TRAINER_BUG_CATCHER_4",
    "TRAINER_BUG_CATCHER_5",
    "TRAINER_BUG_CATCHER_6",
    "TRAINER_BUG_CATCHER_7",
    "TRAINER_BUG_CATCHER_8",
    "TRAINER_BUG_CATCHER_COLTON_2",
    "TRAINER_BUG_CATCHER_COLTON_3",
    "TRAINER_BUG_CATCHER_COLTON_4",
    "TRAINER_BUG_MANIAC",
    "TRAINER_BURGLAR_1",
    "TRAINER_BURGLAR_2",
    "TRAINER_BURGLAR_3",
    "TRAINER_BURGLAR_4",
    "TRAINER_CAMPER_2",
    "TRAINER_CAMPER_CHRIS_2",
    "TRAINER_CAMPER_CHRIS_3",
    "TRAINER_CAMPER_CHRIS_4",
    "TRAINER_CAMPER_JEFF_2",
    "TRAINER_CAMPER_JEFF_3",
    "TRAINER_CAMPER_JEFF_4",
    "TRAINER_CAMPER_RICKY_2",
    "TRAINER_CAMPER_RICKY_3",
    "TRAINER_CAMPER_RICKY_4",
    "TRAINER_CHANNELER_1",
    "TRAINER_CHANNELER_2",
    "TRAINER_CHANNELER_3",
    "TRAINER_CHANNELER_4",
    "TRAINER_CHANNELER_5",
    "TRAINER_CHANNELER_6",
    "TRAINER_CHANNELER_7",
    "TRAINER_CHANNELER_8",
    "TRAINER_COLLECTOR",
    "TRAINER_COOLTRAINER_AUSTINA",
    "TRAINER_COOLTRAINER_BERKE",
    "TRAINER_COOLTRAINER_BROOKE",
    "TRAINER_COOLTRAINER_GILBERT",
    "TRAINER_COOLTRAINER_JULIE",
    "TRAINER_COOLTRAINER_LEROY_2",
    "TRAINER_COOLTRAINER_MICHELLE_2",
    "TRAINER_COOLTRAINER_OWEN",
    "TRAINER_COOLTRAINER_PAUL",
    "TRAINER_COOLTRAINER_SHANNON",
    "TRAINER_COOL_COUPLE_LEX_NYA_2",
    "TRAINER_CRUSH_GIRL_CYNDY_2",
    "TRAINER_CRUSH_GIRL_SHARON_2",
    "TRAINER_CRUSH_GIRL_SHARON_3",
    "TRAINER_CRUSH_GIRL_TANYA_2",
    "TRAINER_CRUSH_GIRL_TANYA_3",
    "TRAINER_CRUSH_KIN_MIK_KIA_2",
    "TRAINER_CRUSH_KIN_MIK_KIA_3",
    "TRAINER_CRUSH_KIN_RON_MYA_2",
    "TRAINER_CRUSH_KIN_RON_MYA_3",
    "TRAINER_CRUSH_KIN_RON_MYA_4",
    "TRAINER_CUE_BALL_CAMRON_2",
    "TRAINER_CUE_BALL_CHASE",
    "TRAINER_CUE_BALL_COREY_2",
    "TRAINER_CUE_BALL_ISAIAH_2",
    "TRAINER_CYCLING_TRIATHLETE_F",
    "TRAINER_CYCLING_TRIATHLETE_M",
    "TRAINER_DRAGON_TAMER",
    "TRAINER_ELITE_FOUR_DRAKE",
    "TRAINER_ELITE_FOUR_GLACIA",
    "TRAINER_ELITE_FOUR_PHOEBE",
    "TRAINER_ELITE_FOUR_SIDNEY",
    "TRAINER_ENGINEER_BERNIE_2",
    "TRAINER_EXPERT_F",
    "TRAINER_EXPERT_M",
    "TRAINER_FISHERMAN_ELLIOT_2",
    "TRAINER_FISHERMAN_WADE_2",
    "TRAINER_GAMER_1",
    "TRAINER_GAMER_DARIAN_2",
    "TRAINER_GAMER_RICH_2",
    "TRAINER_GENTLEMAN_NORTON",
    "TRAINER_GENTLEMAN_WALTER",
    "TRAINER_GUITARIST",
    "TRAINER_HEX_MANIAC",
    "TRAINER_HIKER_EARL_2",
    "TRAINER_HIKER_FRANKLIN_2",
    "TRAINER_HIKER_JEREMY_2",
    "TRAINER_HIKER_TRENT_2",
    "TRAINER_INTERVIEWER",
    "TRAINER_JUGGLER_MASON_2",
    "TRAINER_KINDLER",
    "TRAINER_LASS_2",
    "TRAINER_LASS_JANICE_2",
    "TRAINER_LASS_JANICE_3",
    "TRAINER_LASS_MEGAN_2",
    "TRAINER_LASS_MEGAN_3",
    "TRAINER_LASS_RELI_2",
    "TRAINER_LASS_RELI_3",
    "TRAINER_LEADER_BRAWLY",
    "TRAINER_LEADER_FLANNERY",
    "TRAINER_LEADER_NORMAN",
    "TRAINER_LEADER_ROXANNE",
    "TRAINER_LEADER_TATE_LIZA",
    "TRAINER_LEADER_WALLACE",
    "TRAINER_LEADER_WATTSON",
    "TRAINER_LEADER_WINONA",
    "TRAINER_MAGMA_ADMIN_COURTNEY",
    "TRAINER_MAGMA_ADMIN_TABITHA",
    "TRAINER_MAGMA_GRUNT_M",
    "TRAINER_MAGMA_LEADER",
    "TRAINER_MAMGA_GRUNT_F",
    "TRAINER_MAY_2",
    "TRAINER_MAY_3",
    "TRAINER_MAY",
    "TRAINER_NINJA_BOY",
    "TRAINER_NONE",
    "TRAINER_OLD_COUPLE",
    "TRAINER_PAINTER_RAYNA_2",
    "TRAINER_PARASOL_LADY",
    "TRAINER_PICNICKER_ALICIA_2",
    "TRAINER_PICNICKER_ALICIA_3",
    "TRAINER_PICNICKER_ALICIA_4",
    "TRAINER_PICNICKER_BECKY_2",
    "TRAINER_PICNICKER_BECKY_3",
    "TRAINER_PICNICKER_BECKY_4",
    "TRAINER_PICNICKER_HANNAH",
    "TRAINER_PICNICKER_ISABELLE_2",
    "TRAINER_PICNICKER_ISABELLE_3",
    "TRAINER_PICNICKER_ISABELLE_4",
    "TRAINER_PICNICKER_KELSEY_2",
    "TRAINER_PICNICKER_KELSEY_3",
    "TRAINER_PICNICKER_KELSEY_4",
    "TRAINER_PICNICKER_MISSY_2",
    "TRAINER_PICNICKER_MISSY_3",
    "TRAINER_PICNICKER_SUSIE_2",
    "TRAINER_PICNICKER_SUSIE_3",
    "TRAINER_PICNICKER_SUSIE_4",
    "TRAINER_PKMN_BREEDER_ALIZE_2",
    "TRAINER_PKMN_PROF_PROF_OAK",
    "TRAINER_PKMN_RANGER_JACKSON_2",
    "TRAINER_PKMN_RANGER_KATELYN_2",
    "TRAINER_PKMN_RANGER_MADELINE_2",
    "TRAINER_PKMN_RANGER_NICOLAS_2",
    "TRAINER_PLAYER_BRENDAN",
    "TRAINER_PLAYER_LEAF",
    "TRAINER_PLAYER_MAY",
    "TRAINER_PLAYER_RED",
    "TRAINER_POKEFAN_F",
    "TRAINER_POKEFAN_M",
    "TRAINER_POKEMANIAC_HECTOR_2",
    "TRAINER_POKEMANIAC_HERMAN_2",
    "TRAINER_POKEMANIAC_HERMAN_3",
    "TRAINER_POKEMANIAC_MARK_2",
    "TRAINER_POKEMANIAC_MARK_3",
    "TRAINER_PSYCHIC_DARIO_2",
    "TRAINER_PSYCHIC_JACLYN_2",
    "TRAINER_PSYCHIC_RODETTE_2",
    "TRAINER_RICH_BOY",
    "TRAINER_ROCKER_LUCA_2",
    "TRAINER_ROCKER_RANDALL",
    "TRAINER_RS_AROMA_LADY",
    "TRAINER_RS_BEAUTY",
    "TRAINER_RS_BIRD_KEEPER",
    "TRAINER_RS_BLACK_BELT",
    "TRAINER_RS_BUG_CATCHER",
    "TRAINER_RS_CAMPER",
    "TRAINER_RS_CHAMPION",
    "TRAINER_RS_COOLTRAINER_F",
    "TRAINER_RS_COOLTRAINER_M",
    "TRAINER_RS_FISHERMAN",
    "TRAINER_RS_GENTLEMAN",
    "TRAINER_RS_HIKER",
    "TRAINER_RS_LADY",
    "TRAINER_RS_LASS",
    "TRAINER_RS_PICNICKER",
    "TRAINER_RS_PKMN_BREEDER_F",
    "TRAINER_RS_PKMN_BREEDER_M",
    "TRAINER_RS_PKMN_RANGER_F",
    "TRAINER_RS_PKMN_RANGER_M",
    "TRAINER_RS_POKEMANIAC",
    "TRAINER_RS_PSYCHIC_F",
    "TRAINER_RS_PSYCHIC_M",
    "TRAINER_RS_RUIN_MANIAC",
    "TRAINER_RS_SAILOR",
    "TRAINER_RS_SIS_AND_BRO",
    "TRAINER_RS_SWIMMER_F",
    "TRAINER_RS_SWIMMER_M",
    "TRAINER_RS_TUBER_F",
    "TRAINER_RS_TUBER_M",
    "TRAINER_RS_TWINS",
    "TRAINER_RS_YOUNGSTER",
    "TRAINER_RS_YOUNG_COUPLE",
    "TRAINER_RUIN_MANIAC_LARRY_2",
    "TRAINER_RUNNING_TRIATHLETE_F",
    "TRAINER_RUNNING_TRIATHLETE_M",
    "TRAINER_SCHOOL_KID_F",
    "TRAINER_SCHOOL_KID_M",
    "TRAINER_SIS_AND_BRO_LIL_IAN_2",
    "TRAINER_SIS_AND_BRO_LIL_IAN_3",
    "TRAINER_SR_AND_JR",
    "TRAINER_SUPER_NERD_1",
    "TRAINER_SUPER_NERD_2",
    "TRAINER_SUPER_NERD_3",
    "TRAINER_SUPER_NERD_GLENN_2",
    "TRAINER_SWIMMER_FEMALE_ALICE_2",
    "TRAINER_SWIMMER_FEMALE_MELISSA_2",
    "TRAINER_SWIMMER_FEMALE_NICOLE_2",
    "TRAINER_SWIMMER_MALE_DARRIN_2",
    "TRAINER_SWIMMER_MALE_FINN_2",
    "TRAINER_SWIMMER_MALE_JACK_2",
    "TRAINER_SWIMMER_MALE_MATTHEW_2",
    "TRAINER_SWIMMER_MALE_SAMIR_2",
    "TRAINER_SWIMMER_MALE_TONY_2",
    "TRAINER_SWIMMING_TRIATHLETE_F",
    "TRAINER_SWIMMING_TRIATHLETE_M",
    "TRAINER_TAMER_EVAN_2",
    "TRAINER_TAMER_JOHN",
    "TRAINER_TEAM_ROCKET_GRUNT_22",
    "TRAINER_TUBER_AMIRA_2",
    "TRAINER_TWINS_ELI_ANNE_2",
    "TRAINER_TWINS_JOY_MEG_2",
    "TRAINER_WALLY",
    "TRAINER_YOUNGSTER_BEN_2",
    "TRAINER_YOUNGSTER_BEN_3",
    "TRAINER_YOUNGSTER_BEN_4",
    "TRAINER_YOUNGSTER_CHAD_2",
    "TRAINER_YOUNGSTER_CHAD_3",
    "TRAINER_YOUNGSTER_CHAD_4",
    "TRAINER_YOUNGSTER_DESTIN_2",
    "TRAINER_YOUNGSTER_TIMMY_2",
    "TRAINER_YOUNGSTER_TIMMY_3",
    "TRAINER_YOUNGSTER_TIMMY_4",
    "TRAINER_YOUNGSTER_YASU_2",
    "TRAINER_YOUNGSTER_YASU_3",
    "TRAINER_YOUNG_COUPLE_GIA_JES_2",
    "TRAINER_YOUNG_COUPLE_GIA_JES_3"
};

const std::vector<std::string> FAME_CHECKER_PEOPLE = {
    "FAME_CHECKER_OAK",
    "FAME_CHECKER_DAISY",
    "FAME_CHECKER_BROCK",
    "FAME_CHECKER_MISTY",
    "FAME_CHECKER_LTSURGE",
    "FAME_CHECKER_ERIKA",
    "FAME_CHECKER_KOGA",
    "FAME_CHECKER_SABRINA",
    "FAME_CHECKER_BLAINE",
    "FAME_CHECKER_LORELEI",
    "FAME_CHECKER_BRUNO",
    "FAME_CHECKER_AGATHA",
    "FAME_CHECKER_LANCE",
    "FAME_CHECKER_BILL",
    "FAME_CHECKER_MRFUJI",
    "FAME_CHECKER_GIOVANNI"
};

int main (int argc, char *argv[])
{
    std::filesystem::path root_dir = std::filesystem::path(".");

    if (argc != 9)
    {
        fprintf(stderr, "Not eneough arguments\n");
        exit(1);
    }

    std::string sym_files[4] = {argv[1], argv[2], argv[3], argv[4]};
    std::string rom_files[4] = {argv[5], argv[6], argv[7], argv[8]};
    std::string out_file = "extracted_data.json";

    // ------------------------------------------------------------------------
    // Getting constants
    // ------------------------------------------------------------------------
    std::cout << "Loading constants..." << std::endl;
    std::ifstream macro_file(root_dir / "constants.json");
    if (macro_file.fail())
    {
        fprintf(stderr, "Could not find constants.json\n");
        exit(1);
    }
    json constants_json = json::parse(macro_file);

    std::map<std::string, std::map<std::string, uint32_t>> misc_ram_addresses;
    std::map<std::string, std::map<std::string, uint32_t>> misc_rom_addresses;
    std::map<std::string, std::shared_ptr<LocationInfo>> npc_gifts;
    std::map<std::string, std::shared_ptr<LocationInfo>> fly_unlocks;
    std::map<std::string, std::shared_ptr<LocationInfo>> badges;
    std::map<std::string, std::shared_ptr<LocationInfo>> famechecker_rewards;
    std::map<std::string, std::shared_ptr<LocationInfo>> dex_rewards;
    std::map<std::string, std::shared_ptr<TrainerInfo>> trainers;
    std::map<std::string, std::shared_ptr<LocationInfo>> trainer_rewards;
    std::map<std::string, std::shared_ptr<LocationInfo>> ball_items;
    std::map<std::string, std::shared_ptr<LocationInfo>> hidden_items;
    std::map<std::string, std::shared_ptr<MapInfo>> maps;
    std::vector<std::shared_ptr<WarpInfo>> warps;
    std::map<std::string, std::shared_ptr<StarterPokemonInfo>> starter_pokemon;
    std::map<std::string, std::shared_ptr<StaticPokemonInfo>> misc_pokemon;
    std::map<std::string, std::shared_ptr<StaticPokemonInfo>> legendary_pokemon;
    std::map<int, std::shared_ptr<SpeciesInfo>> all_species;
    uint16_t tmhm_moves[58];
    std::map<std::string, std::string> rom_names;

    std::map<int, std::string> trainer_names;

    for (auto iter = constants_json.begin(); iter != constants_json.end(); iter++)
    {
        if (iter.key().substr(0, 8) == "TRAINER_" &&
            iter.key() != "TRAINER_FLAGS_START" &&
            iter.key() != "TRAINER_FLAGS_END")
        {
            trainer_names[iter.value()] = iter.key();
        }
    }

    for(int i = 0; i < 4; i++)
    {
        // ------------------------------------------------------------------------
        // Reading symbols
        // ------------------------------------------------------------------------
        std::cout << "Reading symbols for " << GAME_NAME_MAP[i] <<  "..." << std::endl;
        std::ifstream symbol_map_file(root_dir / sym_files[i]);
        if (symbol_map_file.fail())
        {
            fprintf(stderr, "Could not find sym file\n");
            exit(1);
        }
        std::regex symbol_map_regex("^([0-9a-fA-F]+) [lg] [0-9a-fA-F]+ ([a-zA-Z0-9_]+)$");
        std::map<std::string, uint32_t> symbol_map;

        std::string line;
        while (std::getline(symbol_map_file, line))
        {
            std::smatch m;
            if (std::regex_match(line, m, symbol_map_regex))
            {
                symbol_map[m[2]] = std::stoi(m[1], nullptr, 16);
            }
        }

        misc_ram_addresses[GAME_REVISION_MAP[i]] = {
            { "CB2_Overworld", symbol_map["CB2_Overworld"] },
            { "gArchipelagoReceivedItem", symbol_map["gArchipelagoReceivedItem"] },
            { "gMain", symbol_map["gMain"] },
            { "gSaveBlock1Ptr", symbol_map["gSaveBlock1Ptr"] },
            { "gSaveBlock2Ptr", symbol_map["gSaveBlock2Ptr"] },
            { "gArchipelagoDeathLinkQueued", symbol_map["gArchipelagoDeathLinkQueued"] },
            { "gPlayerParty", symbol_map["gPlayerParty"] },
            { "gEnemyParty", symbol_map["gEnemyParty"] },
        };

        misc_rom_addresses[GAME_REVISION_MAP[i]] = {
            { "gArchipelagoOptions", symbol_map["gArchipelagoOptions"] - ROM_START },
            { "gArchipelagoStartingItems", symbol_map["gArchipelagoStartingItems"] - ROM_START },
            { "gArchipelagoStartingItemsCount", symbol_map["gArchipelagoStartingItemsCount"] - ROM_START },
            { "gArchipelagoPlayerNames", symbol_map["gArchipelagoPlayerNames"] - ROM_START },
            { "gArchipelagoItemNames", symbol_map["gArchipelagoItemNames"] - ROM_START },
            { "gArchipelagoNameTable", symbol_map["gArchipelagoNameTable"] - ROM_START },
            { "gArchipelagoInfo", symbol_map["gArchipelagoInfo"] - ROM_START },
            { "gBattleMoves", symbol_map["gBattleMoves"] - ROM_START },
            { "gLevelUpLearnsets", symbol_map["gLevelUpLearnsets"] - ROM_START },
            { "gSpeciesInfo", symbol_map["gSpeciesInfo"] - ROM_START },
            { "sStarterSpecies", symbol_map["sStarterSpecies"] - ROM_START },
            { "sTMHMLearnsets", symbol_map["sTMHMLearnsets"] - ROM_START },
            { "gTrainers", symbol_map["gTrainers"] - ROM_START },
            { "sTMHMMoves", symbol_map["sTMHMMoves"] - ROM_START },
            { "gEvolutionTable", symbol_map["gEvolutionTable"] - ROM_START },
            { "gTutorMoves", symbol_map["gTutorMoves"] - ROM_START },
            { "sTutorLearnsets", symbol_map["sTutorLearnsets"] - ROM_START },
            { "sFanfares", symbol_map["sFanfares"] - ROM_START },
            { "sInGameTrades", symbol_map["sInGameTrades"] - ROM_START},
            { "sFlashLevelToRadius", symbol_map["sFlashLevelToRadius"] - ROM_START},
            { "sArchipelagoPCItemId", symbol_map["sArchipelagoPCItemId"] - ROM_START}
        };

        // ------------------------------------------------------------------------
        // Reading ROM
        // ------------------------------------------------------------------------
        std::cout << "Reading ROM for " << GAME_NAME_MAP[i] <<  "..." << std::endl;
        std::ifstream rom(root_dir / rom_files[i], std::ios::binary);
        if (rom.fail())
        {
            fprintf(stderr, "Could not open rom file\n");
            exit(1);
        }

        // NPC Gifts
        for (auto const& [symbol, address] : symbol_map)
        {
            if (symbol.substr(0, 28) == "Archipelago_Target_NPC_Gift_")
            {
                auto npc_gift = npc_gifts["NPC_GIFT_" + symbol.substr(33)];

                if (npc_gift != nullptr)
                {
                    npc_gift->address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;
                }
                else
                {
                    npc_gift = std::make_shared<LocationInfo>();
                    npc_gift->name = "NPC_GIFT_" + symbol.substr(33);
                    npc_gift->flag = constants_json[symbol.substr(28)];
                    npc_gift->address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;
                    rom.seekg(npc_gift->address[GAME_REVISION_MAP[i]], std::ios::beg);
                    rom.read((char*)&(npc_gift->default_item), 2);
                    npc_gifts[npc_gift->name] = npc_gift;
                }
            }
        }

        // Fly Unlocks
        for (auto const& [symbol, address] : symbol_map)
        {
            if (symbol.substr(0, 30) == "Archipelago_Target_Fly_Unlock_")
            {
                auto fly_unlock = fly_unlocks["FLY_UNLOCK_" + symbol.substr(35)];

                if (fly_unlock != nullptr)
                {
                    fly_unlock->address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;
                }
                else
                {
                    fly_unlock = std::make_shared<LocationInfo>();
                    fly_unlock->name = "FLY_UNLOCK_" + symbol.substr(35);
                    fly_unlock->flag = constants_json[symbol.substr(30)];
                    fly_unlock->address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;
                    rom.seekg(fly_unlock->address[GAME_REVISION_MAP[i]], std::ios::beg);
                    rom.read((char*)&(fly_unlock->default_item), 2);
                    fly_unlocks[fly_unlock->name] = fly_unlock;
                }
            }
        }

        // Badges
        for (auto const& [symbol, address] : symbol_map)
        {
            if (symbol.substr(0, 25) == "Archipelago_Target_Badge_")
            {
                auto badge = badges["BADGE_" + symbol.substr(25)];

                if (badge != nullptr)
                {
                    badge->address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;
                }
                else
                {
                    badge = std::make_shared<LocationInfo>();
                    badge->name = "BADGE_" + symbol.substr(25);
                    badge->flag = constants_json["FLAG_RECEIVED_BADGE_" + symbol.substr(25)];
                    badge->address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;
                    rom.seekg(badge->address[GAME_REVISION_MAP[i]], std::ios::beg);
                    rom.read((char*)&(badge->default_item), 2);
                    badges[badge->name] = badge;
                }
            }
        }

        // Fame Checker
        for (size_t j = 0; j < FAME_CHECKER_PEOPLE.size(); j++)
        {
            for (size_t k = 0; k < 6; k++)
            {
                auto num = std::to_string(k + 1);
                auto famechecker_reward = famechecker_rewards[FAME_CHECKER_PEOPLE[j] + "_" + num];

                if (famechecker_reward != nullptr)
                {
                    auto index = (j * 6) + k;
                    famechecker_reward->address[GAME_REVISION_MAP[i]] = symbol_map["sFameCheckerRewards"] + (index * 2) - ROM_START;
                }
                else
                {
                    auto index = (j * 6) + k;
                    famechecker_reward = std::make_shared<LocationInfo>();
                    famechecker_reward->name = FAME_CHECKER_PEOPLE[j] + "_" + num;
                    famechecker_reward->flag = FAMESANITY_FLAGS_START + index;
                    famechecker_reward->address[GAME_REVISION_MAP[i]] = symbol_map["sFameCheckerRewards"] + (index * 2) - ROM_START;
                    famechecker_reward->default_item = constants_json["ITEM_NONE"];
                    famechecker_rewards[famechecker_reward->name] = famechecker_reward;
                }
            }
        }

		// Pokedex Entries
        for (size_t j = 0; j < 386; j++)
        {
            std::string padded_dex_number = std::to_string(j + 1);

            if (padded_dex_number.size() < 3)
            {
              padded_dex_number = std::string(3 - padded_dex_number.size(), '0') + padded_dex_number;
            }

            auto dex_reward = dex_rewards["POKEDEX_REWARD_" + padded_dex_number];

            if (dex_reward != nullptr)
            {
                dex_reward->address[GAME_REVISION_MAP[i]] = symbol_map["sPokedexRewards"] + (j * 2) - ROM_START;
            }
            else
            {
                dex_reward = std::make_shared<LocationInfo>();
                dex_reward->name = "POKEDEX_REWARD_" + padded_dex_number;
                dex_reward->flag = DEXSANITY_FLAGS_START + j;
                dex_reward->address[GAME_REVISION_MAP[i]] = symbol_map["sPokedexRewards"] + (j * 2) - ROM_START;
                dex_reward->default_item = constants_json["ITEM_NONE"];
                dex_rewards[dex_reward->name] = dex_reward;
            }
        }

        // Trainers
        for (auto const& [trainer_id, trainer_name] : trainer_names)
        {
            auto trainer = trainers[trainer_name];

            if (trainer != nullptr)
            {
                trainer->address[GAME_REVISION_MAP[i]] = misc_rom_addresses[GAME_REVISION_MAP[i]]["gTrainers"] + (trainer_id * 0x28);

                rom.seekg(trainer->address[GAME_REVISION_MAP[i]] + 0x24, rom.beg);
                rom.read((char*)&(trainer->party_address[GAME_REVISION_MAP[i]]), 4);
                trainer->party_address[GAME_REVISION_MAP[i]] -= ROM_START;
            }
            else
            {
                trainer = std::make_shared<TrainerInfo>();

                trainer->name = trainer_name;
                trainer->address[GAME_REVISION_MAP[i]] = misc_rom_addresses[GAME_REVISION_MAP[i]]["gTrainers"] + (trainer_id * 0x28);

                uint8_t party_flags;
                rom.seekg(trainer->address[GAME_REVISION_MAP[i]] + 0x0, rom.beg);
                rom.read((char*)&(party_flags), 1);

                uint8_t party_size;
                rom.seekg(trainer->address[GAME_REVISION_MAP[i]] + 0x20, rom.beg);
                rom.read((char*)&(party_size), 1);

                rom.seekg(trainer->address[GAME_REVISION_MAP[i]] + 0x24, rom.beg);
                rom.read((char*)&(trainer->party_address[GAME_REVISION_MAP[i]]), 4);
                trainer->party_address[GAME_REVISION_MAP[i]] -= ROM_START;

                switch (party_flags)
                {
                    case 0b00:
                        trainer->pokemon_data_type = NO_ITEM_DEFAULT_MOVES;
                        break;
                    case 0b01:
                        trainer->pokemon_data_type = NO_ITEM_CUSTOM_MOVES;
                        break;
                    case 0b10:
                        trainer->pokemon_data_type = ITEM_DEFAULT_MOVES;
                        break;
                    case 0b11:
                        trainer->pokemon_data_type = ITEM_CUSTOM_MOVES;
                        break;
                    default:
                        throw new std::exception();
                }

                size_t pokemon_data_size;
                switch (trainer->pokemon_data_type)
                {
                    case NO_ITEM_DEFAULT_MOVES:
                        pokemon_data_size = 8;
                        break;
                    case NO_ITEM_CUSTOM_MOVES:
                        pokemon_data_size = 16;
                        break;
                    case ITEM_DEFAULT_MOVES:
                        pokemon_data_size = 8;
                        break;
                    case ITEM_CUSTOM_MOVES:
                        pokemon_data_size = 16;
                        break;
                }

                size_t moves_offset;
                switch (trainer->pokemon_data_type)
                {
                    case NO_ITEM_CUSTOM_MOVES:
                        moves_offset = 6;
                        break;
                    case ITEM_CUSTOM_MOVES:
                        moves_offset = 8;
                        break;
                    default:
                        moves_offset = 0;
                        break;
                }

                for (size_t k = 0; k < party_size; k++)
                {
                    TrainerPokemonInfo pokemon;

                    uint32_t address = trainer->party_address[GAME_REVISION_MAP[i]] + (k * pokemon_data_size);

                    rom.seekg(address + 2, rom.beg);
                    rom.read((char*)&(pokemon.level), 1);

                    rom.seekg(address + 4, rom.beg);
                    rom.read((char*)&(pokemon.species), 2);

                    if (trainer->pokemon_data_type == NO_ITEM_CUSTOM_MOVES || trainer->pokemon_data_type == ITEM_CUSTOM_MOVES)
                    {
                        for (size_t l = 0; l < 4; l++)
                        {
                            uint16_t move;
                            rom.seekg(address + moves_offset + (l * 2), rom.beg);
                            rom.read((char*)&(move), 2);

                            pokemon.moves[l] = move;
                        }
                    }
                    else
                    {
                        pokemon.moves[0] = 0;
                        pokemon.moves[1] = 0;
                        pokemon.moves[2] = 0;
                        pokemon.moves[3] = 0;
                    }

                    trainer->party.push_back(pokemon);
                }

                trainers[trainer->name] = trainer;
            }

            if (IGNORABLE_TRAINER_REWARDS.find(trainer_name) != IGNORABLE_TRAINER_REWARDS.end())
                continue;

            auto trainer_reward = trainer_rewards[trainer_name + "_REWARD"];

            if (trainer_reward != nullptr)
            {
                trainer_reward->address[GAME_REVISION_MAP[i]] = symbol_map["sTrainerRewards"] + (trainer_id * 2) - ROM_START;
            }
            else
            {
                trainer_reward = std::make_shared<LocationInfo>();

                trainer_reward->name = trainer_name + "_REWARD";
                trainer_reward->flag = trainer_id + (uint16_t)constants_json["TRAINER_FLAGS_START"];
                trainer_reward->default_item = constants_json["ITEM_NONE"];
                trainer_reward->address[GAME_REVISION_MAP[i]] = symbol_map["sTrainerRewards"] + (trainer_id * 2) - ROM_START;

                trainer_rewards[trainer_reward->name] = trainer_reward;
            }
        }

        // ------------------------------------------------------------------------
        // Reading map.json files
        // ------------------------------------------------------------------------
        std::cout << "Reading maps for " << GAME_NAME_MAP[i] <<  "..." << std::endl;

        for(const auto& entry: std::filesystem::directory_iterator(root_dir / "data/maps/"))
        {
            if (entry.is_directory())
            {
                std::ifstream map_file(entry.path() / "map.json");
                json map_data_json = json::parse(map_file);

                auto map = maps[map_data_json["id"]];

                if (map != nullptr)
                {
                    map->header_address[GAME_REVISION_MAP[i]] = symbol_map[(std::string)map_data_json["name"]] - ROM_START;
                    map->warp_table_address[GAME_REVISION_MAP[i]] = symbol_map[(std::string)map_data_json["name"] + "_MapWarps"] - ROM_START;
                }
                else
                {
                    std::shared_ptr<MapInfo> map(new MapInfo());
                    map->name = map_data_json["id"];
                    maps[map->name] = map;
                    map->header_address[GAME_REVISION_MAP[i]] = symbol_map[(std::string)map_data_json["name"]] - ROM_START;

                    // ----------------------------------------------------------------
                    // Warps
                    // ----------------------------------------------------------------

                    // Many warps are actually two or three events acting as one logical warp.
                    // Doorways, for example, are often 2 tiles wide indoors but
                    // only 1 tile wide outdoors. Both indoor warps point to the
                    // outdoor warp, and the outdoor warp points to only one of the
                    // indoor warps. There are also warps that are 2 tiles wide and lead to
                    // a corresponding pair of warps. We want to describe warps logically in a way that
                    // retains information about individual warp events.
                    //
                    // This is how warps are encoded:
                    //
                    // {source_map}:{source_warp_ids}/{dest_map}:{dest_warp_id}[!]
                    //    source_map:       The map the warp events are located in
                    //    source_warp_ids:  The ids of all adjacent warp events in source_map
                    //                      (these must be in ascending order)
                    //    dest_map:         The map of the warp event to which this one is connected
                    //    dest_warp_ids:     The ids of the warp events in dest_map
                    //    [!]:              If the warp expects to lead to a destination which does
                    //                      not lead back to it, add a ! to the end
                    //
                    // Example:   MAP_VIRIDIAN_FOREST:0,1,2/MAP_ROUTE2_VIRIDIAN_FOREST_SOUTH_ENTRANCE:1
                    // Example 2: MAP_SEAFOAM_ISLANDS_B2F:9/MAP_SEAFOAM_ISLANDS_B3F:5!
                    //
                    // Note: A warp must have its destination set as another warp event.
                    // However, that does not guarantee that the destination warp event
                    // will warp back to the source. There are (few) one-way warps.
                    //
                    // Note2: Some warp destinations go to the map "MAP_DYNAMIC" and
                    // have a warp id which is not a number. These edge cases are:
                    //   - The Department Store Elevator
                    //   - The Rocket Hideout Elevator
                    //   - The Silph Co. Elevator
                    //   - The Trainer Tower Elevator
                    //   - The Trade Center
                    //   - The Union Room
                    //   - The Record Corner
                    //   - 2P/4P Battle Colosseum

                    map->warp_table_address[GAME_REVISION_MAP[i]] = symbol_map[(std::string)map_data_json["name"] + "_MapWarps"] - ROM_START;

                    json warp_events_json = map_data_json["warp_events"];

                    // (id, x, y, destination_map, destination_id)
                    std::vector<std::shared_ptr<WarpInfo>> map_warps;
                    uint i = 0;
                    for (const auto& warp_json: warp_events_json)
                    {
                        std::shared_ptr<WarpInfo> warp(new WarpInfo());
                        warp->source_map = map->name;
                        warp->source_indices.push_back(i);
                        warp->source_coordinates.push_back(std::tuple<int, int>(warp_json["x"], warp_json["y"]));
                        warp->dest_map = warp_json["dest_map"];
                        if (warp_json["dest_warp_id"] == "WARP_ID_DYNAMIC")
                        {
                            warp->dest_indices.push_back(-1);
                        }
                        else if (warp_json["dest_warp_id"] == "WARP_ID_SECRET_BASE")
                        {
                            warp->dest_indices.push_back(-2);
                        }
                        else
                        {
                            warp->dest_indices.push_back(std::stoi(static_cast<std::string>(warp_json["dest_warp_id"])));
                        }

                        map_warps.push_back(warp);
                        ++i;
                    }

                    // Sort so that adjacency checker only needs to check against the
                    // previously found matching warp. Otherwise would have to do a
                    // recursive flood of some sort.
                    std::sort(
                        map_warps.begin(), map_warps.end(),
                        [](std::shared_ptr<WarpInfo> a, std::shared_ptr<WarpInfo> b)
                        {
                            return std::get<0>(a->source_coordinates[0]) == std::get<0>(b->source_coordinates[0])
                                ? std::get<1>(a->source_coordinates[0]) < std::get<1>(b->source_coordinates[0])
                                : std::get<0>(a->source_coordinates[0]) < std::get<0>(b->source_coordinates[0]);
                        }
                    );

                    // Group warps by whether they're logically the same
                    std::vector<std::shared_ptr<WarpInfo>> grouped_warps;
                    std::vector<bool> is_collected(map_warps.size());
                    for (uint i = 0; i < map_warps.size(); ++i)
                    {
                        if (is_collected[i]) continue;

                        const auto warp = map_warps[i];
                        is_collected[i] = true;

                        for (uint j = i + 1; j < map_warps.size(); ++j)
                        {
                            const auto other_warp = map_warps[j];

                            // Check destination map to exit early, but we're assuming that adjacent
                            // warps are always part of the same logical warp
                            if (warp->dest_map != other_warp->dest_map) continue;
                            // Ignore the dropdowns on Seafoam Island B3F & B4F as their destinations are not adjacent
                            if ((warp->source_map == "MAP_SEAFOAM_ISLANDS_B3F" &&
                                 std::find(warp->source_indices.begin(), warp->source_indices.end(), 5) != warp->source_indices.end()) ||
                                (warp->source_map == "MAP_SEAFOAM_ISLANDS_B3F" &&
                                 std::find(warp->source_indices.begin(), warp->source_indices.end(), 6) != warp->source_indices.end()) ||
                                (warp->source_map == "MAP_SEAFOAM_ISLANDS_B4F" &&
                                 std::find(warp->source_indices.begin(), warp->source_indices.end(), 2) != warp->source_indices.end()) ||
                                (warp->source_map == "MAP_SEAFOAM_ISLANDS_B4F" &&
                                 std::find(warp->source_indices.begin(), warp->source_indices.end(), 3) != warp->source_indices.end()))
                                 continue;
                            // Check adjacency
                            if (
                                abs(std::get<0>(warp->source_coordinates.back()) - std::get<0>(other_warp->source_coordinates[0])) +
                                abs(std::get<1>(warp->source_coordinates.back()) - std::get<1>(other_warp->source_coordinates[0])) > 1

                            ) continue;

                            warp->source_indices.push_back(other_warp->source_indices[0]);
                            warp->dest_indices.push_back(other_warp->dest_indices[0]);
                            warp->source_coordinates.push_back(other_warp->source_coordinates[0]);
                            is_collected[j] = true;
                        }
                        grouped_warps.push_back(warp);
                    }

                    for (const auto &warp: grouped_warps)
                    {
                        warps.push_back(warp);
                    }
                }

                // ----------------------------------------------------------------
                // Items
                // ----------------------------------------------------------------
                json object_events_json = map_data_json["object_events"];
                for (const auto& event_json: object_events_json)
                {
                    if (event_json["type"] == "object")
                    {
                        std::string flag_name = event_json["flag"].get<std::string>();
                        if (flag_name.substr(0, 9) == "FLAG_ITEM")
                        {
                            auto item = ball_items[flag_name.substr(5)];

                            if (item != nullptr)
                            {
                                item->address[GAME_REVISION_MAP[i]] = symbol_map[event_json["script"]] + 3 - ROM_START;
                            }
                            else
                            {
                                item = std::make_shared<LocationInfo>();
                                item->flag = constants_json[flag_name];
                                item->name = flag_name.substr(5);
                                item->address[GAME_REVISION_MAP[i]] = symbol_map[event_json["script"]] + 3 - ROM_START;
                                rom.seekg(item->address[GAME_REVISION_MAP[i]], rom.beg);
                                rom.read((char*)&(item->default_item), 2);
                                ball_items[item->name] = item;
                            }
                        }
                    }
                }

                json bg_events_json = map_data_json["bg_events"];
                for (const auto& event_json: bg_events_json)
                {
                    if (event_json["type"] == "hidden_item")
                    {
                        std::string flag_name = event_json["flag"].get<std::string>();
                        auto item = hidden_items[flag_name.substr(5)];

                        if (item != nullptr)
                        {
                            item->address[GAME_REVISION_MAP[i]] = symbol_map["Archipelago_Target_Hidden_Item_" + flag_name] + 8 - ROM_START;
                        }
                        else
                        {
                            item = std::make_shared<LocationInfo>();
                            item->flag = constants_json[flag_name];
                            item->name = flag_name.substr(5);
                            item->address[GAME_REVISION_MAP[i]] = symbol_map["Archipelago_Target_Hidden_Item_" + flag_name] + 8 - ROM_START;
                            item->default_item = constants_json[event_json["item"].get<std::string>()];
                            hidden_items[item->name] = item;
                        }
                    }
                }
            }
        }

        // ------------------------------------------------------------------------
        // Reading encounter tables
        // ------------------------------------------------------------------------
        std::cout << "Reading encounter tables for " << GAME_NAME_MAP[i] << "..." << std::endl;
        std::ifstream wild_encounters_file(root_dir / "src/data/wild_encounters.json");
        json wild_encounters_json = json::parse(wild_encounters_file);

        for (const auto& map_json: wild_encounters_json["wild_encounter_groups"][0]["encounters"]) {
            std::shared_ptr<MapInfo> map = maps[map_json["map"]];

            // Altering Cave is the only map with multiple encounter tables.
            // It is supposed to switch between them based on a value set by an unreleased event.
            // The only vanilla table is the first one, with all Zubats.
            if (map->name == "MAP_SIX_ISLAND_ALTERING_CAVE")
            {
                bool first_encounter_table = false;

                if(map_json["base_label"] == "sSixIslandAlteringCave_FireRed")
                {
                    first_encounter_table = true;
                }
                else if(map_json["base_label"] == "sSixIslandAlteringCave_LeafGreen")
                {
                    first_encounter_table = true;
                }

                if(!first_encounter_table)
                    continue;
            }

            std::string base_symbol = map_json["base_label"];

            // Check that the encounter table is for the game we are extracting
            if ((base_symbol.find("_LeafGreen") != std::string::npos && rom_files[i].substr(0, 11) == "pokefirered") ||
                (base_symbol.find("_FireRed") != std::string::npos && rom_files[i].substr(0, 13) == "pokeleafgreen")) continue;

            map->land_encounters.ram_address[GAME_REVISION_MAP[i]] = symbol_map[base_symbol + "_LandMons"];
            map->land_encounters.address[GAME_REVISION_MAP[i]] = map->land_encounters.ram_address[GAME_REVISION_MAP[i]] - ROM_START;
            map->water_encounters.ram_address[GAME_REVISION_MAP[i]] = symbol_map[base_symbol + "_WaterMons"];
            map->water_encounters.address[GAME_REVISION_MAP[i]] = map->water_encounters.ram_address[GAME_REVISION_MAP[i]] - ROM_START;
            map->fishing_encounters.ram_address[GAME_REVISION_MAP[i]] = symbol_map[base_symbol + "_FishingMons"];
            map->fishing_encounters.address[GAME_REVISION_MAP[i]] = map->fishing_encounters.ram_address[GAME_REVISION_MAP[i]] - ROM_START;

            auto land_slots = map->land_encounters.encounter_slots[GAME_VERSION_MAP[i]];

            if (land_slots == nullptr)
            {
                land_slots = std::make_shared<std::vector<std::shared_ptr<EncounterSlotInfo>>>();
                try
                {
                    for (const auto& encounter_slot_json: map_json.at("land_mons")["mons"]) {
                        auto slot = std::make_shared<EncounterSlotInfo>();
                        land_slots->push_back(slot);
                        slot->default_species = constants_json[encounter_slot_json["species"].get<std::string>()];
                        slot->min_level = encounter_slot_json["min_level"];
                        slot->max_level = encounter_slot_json["max_level"];
                    }
                    map->land_encounters.exists = true;
                }
                catch (const json::exception &e)
                {
                    if (e.id != 403) {
                        throw e;
                    }
                }

                map->land_encounters.encounter_slots[GAME_VERSION_MAP[i]] = land_slots;
            }

            auto water_slots = map->water_encounters.encounter_slots[GAME_VERSION_MAP[i]];

            if (water_slots == nullptr)
            {
                water_slots = std::make_shared<std::vector<std::shared_ptr<EncounterSlotInfo>>>();
                try
                {
                    for (const auto& encounter_slot_json: map_json.at("water_mons")["mons"]) {
                        auto slot = std::make_shared<EncounterSlotInfo>();
                        water_slots->push_back(slot);
                        slot->default_species = constants_json[encounter_slot_json["species"].get<std::string>()];
                        slot->min_level = encounter_slot_json["min_level"];
                        slot->max_level = encounter_slot_json["max_level"];
                    }
                    map->water_encounters.exists = true;
                }
                catch (const json::exception &e)
                {
                    if (e.id != 403) {
                        throw e;
                    }
                }

                map->water_encounters.encounter_slots[GAME_VERSION_MAP[i]] = water_slots;
            }

            auto fishing_slots = map->fishing_encounters.encounter_slots[GAME_VERSION_MAP[i]];

            if (fishing_slots == nullptr)
            {
                fishing_slots = std::make_shared<std::vector<std::shared_ptr<EncounterSlotInfo>>>();
                try
                {
                    for (const auto& encounter_slot_json: map_json.at("fishing_mons")["mons"]) {
                        auto slot = std::make_shared<EncounterSlotInfo>();
                        fishing_slots->push_back(slot);
                        slot->default_species = constants_json[encounter_slot_json["species"].get<std::string>()];
                        slot->min_level = encounter_slot_json["min_level"];
                        slot->max_level = encounter_slot_json["max_level"];
                    }
                    map->fishing_encounters.exists = true;
                }
                catch (const json::exception &e)
                {
                    if (e.id != 403) {
                        throw e;
                    }
                }

                map->fishing_encounters.encounter_slots[GAME_VERSION_MAP[i]] = fishing_slots;
            }
        }

        // Reading player starters
        for (auto const& [symbol, address] : symbol_map)
        {
            if (symbol.substr(0, 34) == "Archipelago_Target_Starter_Player_")
            {
                auto starter = starter_pokemon["STARTER_POKEMON_" + symbol.substr(34)];

                if (starter != nullptr)
                {
                    starter->player_address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;
                }
                else
                {
                    starter = std::make_shared<StarterPokemonInfo>();

                    starter->name = "STARTER_POKEMON_" + symbol.substr(34);
                    starter->player_address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;
                    rom.seekg(starter->player_address[GAME_REVISION_MAP[i]], rom.beg);
                    rom.read((char*)&(starter->species), 2);
                    starter_pokemon[starter->name] = starter;
                }
            }
        }

        // Reading rival starters
        for (auto const& [symbol, address] : symbol_map)
        {
            if (symbol.substr(0, 33) == "Archipelago_Target_Starter_Rival_")
            {
                auto starter = starter_pokemon["STARTER_POKEMON_" + symbol.substr(33)];
                starter->rival_address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;
            }
        }

        // Reading gift pokemon
        for (auto const& [symbol, address] : symbol_map)
        {
            if (symbol.substr(0, 32) == "Archipelago_Target_Special_Gift_")
            {
                auto gift_pokemon = misc_pokemon["GIFT_POKEMON_" + symbol.substr(32)];

                if (gift_pokemon != nullptr)
                {
                    gift_pokemon->address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;

                    if (gift_pokemon->species.find(GAME_VERSION_MAP[i]) == gift_pokemon->species.end())
                    {
                        rom.seekg(gift_pokemon->address[GAME_REVISION_MAP[i]], rom.beg);
                        rom.read((char*)&(gift_pokemon->species[GAME_VERSION_MAP[i]]), 2);
                    }
                }
                else
                {
                    gift_pokemon = std::make_shared<StaticPokemonInfo>();

                    gift_pokemon->name = "GIFT_POKEMON_" + symbol.substr(32);
                    gift_pokemon->address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;
                    rom.seekg(gift_pokemon->address[GAME_REVISION_MAP[i]], rom.beg);
                    rom.read((char*)&(gift_pokemon->species[GAME_VERSION_MAP[i]]), 2);
                    misc_pokemon[gift_pokemon->name] = gift_pokemon;
                }
            }
            else if (symbol.substr(0, 38) == "Archipelago_Target_Level_Special_Gift_")
            {
                auto gift_pokemon = misc_pokemon["GIFT_POKEMON_" + symbol.substr(38)];

                if(gift_pokemon != nullptr)
                {
                    gift_pokemon->level_address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;

                    if (gift_pokemon->level.find(GAME_VERSION_MAP[i]) == gift_pokemon->level.end())
                    {
                        rom.seekg(gift_pokemon->level_address[GAME_REVISION_MAP[i]], rom.beg);
                        rom.read((char*)&(gift_pokemon->level[GAME_VERSION_MAP[i]]), 1);
                    }
                }
                else
                {
                    gift_pokemon = std::make_shared<StaticPokemonInfo>();

                    gift_pokemon->name = "GIFT_POKEMON_" + symbol.substr(38);
                    gift_pokemon->level_address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;
                    rom.seekg(gift_pokemon->level_address[GAME_REVISION_MAP[i]], rom.beg);
                    rom.read((char*)&(gift_pokemon->level[GAME_VERSION_MAP[i]]), 1);
                    misc_pokemon[gift_pokemon->name] = gift_pokemon;
                }
            }
            else if (symbol.substr(0, 33) == "Archipelago_Target_Prize_Pokemon_")
            {
                auto prize_pokemon = misc_pokemon["CELADON_PRIZE_POKEMON_" + symbol.substr(33)];

                if (prize_pokemon != nullptr)
                {
                    prize_pokemon->address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;

                    if (prize_pokemon->species.find(GAME_VERSION_MAP[i]) == prize_pokemon->species.end())
                    {
                        rom.seekg(prize_pokemon->address[GAME_REVISION_MAP[i]], rom.beg);
                        rom.read((char*)&(prize_pokemon->species[GAME_VERSION_MAP[i]]), 2);
                    }
                }
                else
                {
                    prize_pokemon = std::make_shared<StaticPokemonInfo>();

                    prize_pokemon->name = "CELADON_PRIZE_POKEMON_" + symbol.substr(33);
                    prize_pokemon->address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;
                    rom.seekg(prize_pokemon->address[GAME_REVISION_MAP[i]], rom.beg);
                    rom.read((char*)&(prize_pokemon->species[GAME_VERSION_MAP[i]]), 2);
                    misc_pokemon[prize_pokemon->name] = prize_pokemon;
                }
            }
            else if (symbol.substr(0, 39) == "Archipelago_Target_Level_Prize_Pokemon_")
            {
                auto prize_pokemon = misc_pokemon["CELADON_PRIZE_POKEMON_" + symbol.substr(39)];

                if(prize_pokemon != nullptr)
                {
                    prize_pokemon->level_address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;

                    if (prize_pokemon->level.find(GAME_VERSION_MAP[i]) == prize_pokemon->level.end())
                    {
                        rom.seekg(prize_pokemon->level_address[GAME_REVISION_MAP[i]], rom.beg);
                        rom.read((char*)&(prize_pokemon->level[GAME_VERSION_MAP[i]]), 1);
                    }
                }
                else
                {
                    prize_pokemon = std::make_shared<StaticPokemonInfo>();

                    prize_pokemon->name = "CELADON_PRIZE_POKEMON_" + symbol.substr(39);
                    prize_pokemon->level_address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;
                    rom.seekg(prize_pokemon->level_address[GAME_REVISION_MAP[i]], rom.beg);
                    rom.read((char*)&(prize_pokemon->level[GAME_VERSION_MAP[i]]), 1);
                    misc_pokemon[prize_pokemon->name] = prize_pokemon;
                }
            }
            else if (symbol.substr(0, 36) == "Archipelago_Target_Special_Egg_Gift_")
            {
                auto egg_pokemon = misc_pokemon["EGG_POKEMON_" + symbol.substr(36)];

                if (egg_pokemon != nullptr)
                {
                    egg_pokemon->address[GAME_REVISION_MAP[i]] = address + 1 - ROM_START;
                    egg_pokemon->level_address[GAME_REVISION_MAP[i]] = 0;

                    if (egg_pokemon->species.find(GAME_VERSION_MAP[i]) == egg_pokemon->species.end())
                    {
                        rom.seekg(egg_pokemon->address[GAME_REVISION_MAP[i]], rom.beg);
                        rom.read((char*)&(egg_pokemon->species[GAME_VERSION_MAP[i]]), 2);
                    }

                    if (egg_pokemon->level.find(GAME_VERSION_MAP[i]) == egg_pokemon->level.end())
                    {
                        egg_pokemon->level[GAME_VERSION_MAP[i]] = 0;
                    }
                }
                else
                {
                    egg_pokemon = std::make_shared<StaticPokemonInfo>();

                    egg_pokemon->name = "EGG_POKEMON_" + symbol.substr(36);
                    egg_pokemon->address[GAME_REVISION_MAP[i]] = address + 1 - ROM_START;
                    egg_pokemon->level_address[GAME_REVISION_MAP[i]] = 0;
                    egg_pokemon->level[GAME_VERSION_MAP[i]] = 0;
                    rom.seekg(egg_pokemon->address[GAME_REVISION_MAP[i]], rom.beg);
                    rom.read((char*)&(egg_pokemon->species[GAME_VERSION_MAP[i]]), 2);
                    misc_pokemon[egg_pokemon->name] = egg_pokemon;
                }
            }
        }

        // Reading trade pokemon
        for (size_t j = 0; j < TRADE_POKEMON_MAP.size(); j++)
        {
            auto trade_pokemon = misc_pokemon[TRADE_POKEMON_MAP[j]];

            if (trade_pokemon != nullptr)
            {
                trade_pokemon->address[GAME_REVISION_MAP[i]] = misc_rom_addresses[GAME_REVISION_MAP[i]]["sInGameTrades"] + 12 + (j * 60);
                trade_pokemon->level_address[GAME_REVISION_MAP[i]] = 0;

                if (trade_pokemon->species.find(GAME_VERSION_MAP[i]) == trade_pokemon->species.end())
                {
                    rom.seekg(trade_pokemon->address[GAME_REVISION_MAP[i]], rom.beg);
                    rom.read((char*)&(trade_pokemon->species[GAME_VERSION_MAP[i]]), 2);
                }

                if (trade_pokemon->level.find(GAME_VERSION_MAP[i]) == trade_pokemon->level.end())
                {
                    trade_pokemon->level[GAME_VERSION_MAP[i]] = 0;
                }
            }
            else
            {
                trade_pokemon = std::make_shared<StaticPokemonInfo>();

                trade_pokemon->name = TRADE_POKEMON_MAP[j];
                trade_pokemon->address[GAME_REVISION_MAP[i]] = misc_rom_addresses[GAME_REVISION_MAP[i]]["sInGameTrades"] + 12 + (j * 60);
                trade_pokemon->level_address[GAME_REVISION_MAP[i]] = 0;
                trade_pokemon->level[GAME_VERSION_MAP[i]] = 0;
                rom.seekg(trade_pokemon->address[GAME_REVISION_MAP[i]], rom.beg);
                rom.read((char*)&(trade_pokemon->species[GAME_VERSION_MAP[i]]), 2);
                misc_pokemon[trade_pokemon->name] = trade_pokemon;
            }
        }

        // Reading static encounters
        for (auto const& [symbol, address] : symbol_map)
        {
            if (symbol.substr(0, 36) == "Archipelago_Target_Static_Encounter_")
            {
                auto static_encounter = misc_pokemon["STATIC_POKEMON_" + symbol.substr(36)];

                if (static_encounter != nullptr)
                {
                    static_encounter->address[GAME_REVISION_MAP[i]] = address + 1 - ROM_START;
                    static_encounter->level_address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;

                    if (static_encounter->species.find(GAME_VERSION_MAP[i]) == static_encounter->species.end())
                    {
                        rom.seekg(static_encounter->address[GAME_REVISION_MAP[i]], rom.beg);
                        rom.read((char*)&(static_encounter->species[GAME_VERSION_MAP[i]]), 2);
                    }

                    if (static_encounter->level.find(GAME_VERSION_MAP[i]) == static_encounter->level.end())
                    {
                        rom.seekg(static_encounter->level_address[GAME_REVISION_MAP[i]], rom.beg);
                        rom.read((char*)&(static_encounter->level[GAME_VERSION_MAP[i]]), 1);
                    }
                }
                else
                {
                    static_encounter = std::make_shared<StaticPokemonInfo>();

                    static_encounter->name = "STATIC_POKEMON_" + symbol.substr(36);
                    static_encounter->address[GAME_REVISION_MAP[i]] = address + 1 - ROM_START;
                    static_encounter->level_address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;
                    rom.seekg(static_encounter->address[GAME_REVISION_MAP[i]], rom.beg);
                    rom.read((char*)&(static_encounter->species[GAME_VERSION_MAP[i]]), 2);
                    rom.read((char*)&(static_encounter->level[GAME_VERSION_MAP[i]]), 1);
                    misc_pokemon[static_encounter->name] = static_encounter;
                }
            }
        }

        // Reading legendary encounters
        for (auto const& [symbol, address] : symbol_map)
        {
            if (symbol.substr(0, 39) == "Archipelago_Target_Legendary_Encounter_")
            {
                auto legendary_encounter = legendary_pokemon["LEGENDARY_POKEMON_" + symbol.substr(39)];

                if (legendary_encounter != nullptr)
                {
                    legendary_encounter->address[GAME_REVISION_MAP[i]] = address + 1 - ROM_START;
                    legendary_encounter->level_address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;

                    if (legendary_encounter->species.find(GAME_VERSION_MAP[i]) == legendary_encounter->species.end())
                    {
                        rom.seekg(legendary_encounter->address[GAME_REVISION_MAP[i]], rom.beg);
                        rom.read((char*)&(legendary_encounter->species[GAME_VERSION_MAP[i]]), 2);
                    }

                    if (legendary_encounter->level.find(GAME_VERSION_MAP[i]) == legendary_encounter->level.end())
                    {
                        rom.seekg(legendary_encounter->level_address[GAME_REVISION_MAP[i]], rom.beg);
                        rom.read((char*)&(legendary_encounter->level[GAME_VERSION_MAP[i]]), 1);
                    }
                }
                else
                {
                    legendary_encounter = std::make_shared<StaticPokemonInfo>();

                    legendary_encounter->name = "LEGENDARY_POKEMON_" + symbol.substr(39);
                    legendary_encounter->address[GAME_REVISION_MAP[i]] = address + 1 - ROM_START;
                    legendary_encounter->level_address[GAME_REVISION_MAP[i]] = address + 3 - ROM_START;
                    rom.seekg(legendary_encounter->address[GAME_REVISION_MAP[i]], rom.beg);
                    rom.read((char*)&(legendary_encounter->species[GAME_VERSION_MAP[i]]), 2);
                    rom.read((char*)&(legendary_encounter->level[GAME_VERSION_MAP[i]]), 1);
                    legendary_pokemon[legendary_encounter->name] = legendary_encounter;
                }
            }
        }

        // Reading species info
        for (size_t j = 0; j < constants_json["NUM_SPECIES"]; j++)
        {
            auto species = all_species[j];

            if (species != nullptr)
            {
                species->address[GAME_REVISION_MAP[i]] = misc_rom_addresses[GAME_REVISION_MAP[i]]["gSpeciesInfo"] + (j * 28);
            }
            else
            {
                species = std::make_shared<SpeciesInfo>();

                species->id = j;
                species->address[GAME_REVISION_MAP[i]] = misc_rom_addresses[GAME_REVISION_MAP[i]]["gSpeciesInfo"] + (j * 28);

                // Base Stats
                rom.seekg(species->address[GAME_REVISION_MAP[i]] + 0, rom.beg);
                rom.read((char*)&(species->base_stats[0]), 1);
                rom.seekg(species->address[GAME_REVISION_MAP[i]] + 1, rom.beg);
                rom.read((char*)&(species->base_stats[1]), 1);
                rom.seekg(species->address[GAME_REVISION_MAP[i]] + 2, rom.beg);
                rom.read((char*)&(species->base_stats[2]), 1);
                rom.seekg(species->address[GAME_REVISION_MAP[i]] + 3, rom.beg);
                rom.read((char*)&(species->base_stats[3]), 1);
                rom.seekg(species->address[GAME_REVISION_MAP[i]] + 4, rom.beg);
                rom.read((char*)&(species->base_stats[4]), 1);
                rom.seekg(species->address[GAME_REVISION_MAP[i]] + 5, rom.beg);
                rom.read((char*)&(species->base_stats[5]), 1);

                // Types
                rom.seekg(species->address[GAME_REVISION_MAP[i]] + 6, rom.beg);
                rom.read((char*)&(species->types[0]), 1);
                rom.seekg(species->address[GAME_REVISION_MAP[i]] + 7, rom.beg);
                rom.read((char*)&(species->types[1]), 1);

                // Catch Rate
                rom.seekg(species->address[GAME_REVISION_MAP[i]] + 8, rom.beg);
                rom.read((char*)&(species->catch_rate), 1);

                // Friendship
                rom.seekg(species->address[GAME_REVISION_MAP[i]] + 18, rom.beg);
                rom.read((char*)&(species->friendship), 1);

                // Abilities
                rom.seekg(species->address[GAME_REVISION_MAP[i]] + 22, rom.beg);
                rom.read((char*)&(species->abilities[0]), 1);
                rom.seekg(species->address[GAME_REVISION_MAP[i]] + 23, rom.beg);
                rom.read((char*)&(species->abilities[1]), 1);

                all_species[j] = species;
            }
        }

        // Reading learnsets
        for (size_t j = 0; j < constants_json["NUM_SPECIES"]; j++)
        {
            auto species = all_species[j];

            uint32_t learnset_pointer;
            rom.seekg(misc_rom_addresses[GAME_REVISION_MAP[i]]["gLevelUpLearnsets"] + (j * 4), rom.beg);
            rom.read((char*)&(learnset_pointer), 4);
            learnset_pointer -= ROM_START;
            species->learnset_info.address[GAME_REVISION_MAP[i]] = learnset_pointer;

            if (i == 0) // Only on first pass
            {
                uint16_t move;
                size_t move_i = 0;
                do
                {
                    rom.seekg(learnset_pointer + (move_i * 2), rom.beg);
                    rom.read((char*)&(move), 2);

                    if (move != 0xFFFF)
                    {
                        uint8_t level = move >> 9;
                        uint16_t move_id = move & 0x1FF;

                        species->learnset_info.moves.push_back(std::tuple<uint8_t, uint16_t>(level, move_id));
                    }

                    move_i++;
                }
                while (move != 0xFFFF);
            }
        }

        // Reading TM/HM learnsets
        if (i == 0) // Only on first pass
        {
            for (size_t j = 0; j < constants_json["NUM_SPECIES"]; j++)
            {
                auto species = all_species[j];

                rom.seekg(misc_rom_addresses[GAME_REVISION_MAP[i]]["sTMHMLearnsets"] + (j * 8), rom.beg);
                rom.read((char*)&(species->tmhm_learnset), 8);
            }
        }

        // Reading evolutions
        for (size_t j = 0; j < constants_json["NUM_SPECIES"]; j++)
        {
            const size_t NUM_EVOS_PER_MON = 5;
            auto species = all_species[j];

            species->evolutions_address[GAME_REVISION_MAP[i]] = misc_rom_addresses[GAME_REVISION_MAP[i]]["gEvolutionTable"] + (j * (8 * 5));

            if (i == 0) // Only on first pass
            {
                for (size_t k = 0; k < NUM_EVOS_PER_MON; k++)
                {
                    uint16_t method = 0;
                    rom.seekg(species->evolutions_address[GAME_REVISION_MAP[i]] + (k * 8) + 0, rom.beg);
                    rom.read((char*)&(method), 2);

                    if (method == 0) continue;

                    EvolutionInfo evolution;

                    switch (method)
                    {
                        case 1:
                            evolution.method = FRIENDSHIP;
                            break;
                        case 4:
                            evolution.method = LEVEL;
                            break;
                        case 7:
                            evolution.method = ITEM;
                            break;
                        case 8:
                            evolution.method = LEVEL_ATK_GT_DEF;
                            break;
                        case 9:
                            evolution.method = LEVEL_ATK_EQ_DEF;
                            break;
                        case 10:
                            evolution.method = LEVEL_ATK_LT_DEF;
                            break;
                        case 11:
                            evolution.method = LEVEL_SILCOON;
                            break;
                        case 12:
                            evolution.method = LEVEL_CASCOON;
                            break;
                        case 13:
                            evolution.method = LEVEL_NINJASK;
                            break;
                        case 14:
                            evolution.method = LEVEL_SHEDINJA;
                            break;
                        default:
                            std::cerr << "Unknown evolution method: " << method << std::endl;
                            throw new std::exception();
                    }

                    rom.seekg(species->evolutions_address[GAME_REVISION_MAP[i]] + (k * 8) + 2, rom.beg);
                    rom.read((char*)&(evolution.param), 2);

                    rom.seekg(species->evolutions_address[GAME_REVISION_MAP[i]] + (k * 8) + 4, rom.beg);
                    rom.read((char*)&(evolution.species), 2);

                    species->evolutions.push_back(evolution);
                }
            }
        }

        // Reading TM moves
        if (i == 0) // Only on first pass
        {
            for (size_t j = 0; j < 58; j++)
            {
                rom.seekg(misc_rom_addresses[GAME_REVISION_MAP[i]]["sTMHMMoves"] + (j * 2), rom.beg);
                rom.read((char*)&(tmhm_moves[j]), 2);
            }
        }

        // Read ROM name
        char rom_name[32];
        rom.seekg(symbol_map["sGFRomHeader"] - ROM_START + 8, rom.beg);
        rom.read((char*)&(rom_name), 32);
        rom_names[GAME_REVISION_MAP[i]] = rom_name;
    }

    // Now that all warps are created we can check 1-way
    for (const auto &warp: warps)
    {
        for (const auto &other_warp: warps)
        {
            if (warp == other_warp) continue;
            if (warp->connects_to(*other_warp))
            {
                // Found our destination
                if (other_warp->connects_to(*warp))
                {
                    warp->is_one_way = false;
                }

                break;
            }
        }
    }

    // ------------------------------------------------------------------------
    // Creating output
    // ------------------------------------------------------------------------
    std::cout << "Creating JSON..." << std::endl;
    json maps_json;
    for (const auto& [name, map]: maps)
    {
        maps_json[name] = map->to_json();
    }

    json starter_pokemon_json;
    for (const auto& [name, mon]: starter_pokemon)
    {
        starter_pokemon_json[mon->name] = mon->to_json();
    }

    json misc_pokemon_json;
    for (const auto& [name, mon]: misc_pokemon)
    {
        misc_pokemon_json[mon->name] = mon->to_json();
    }

    json legendary_pokemon_json;
    for (const auto& [name, mon]: legendary_pokemon)
    {
        legendary_pokemon_json[mon->name] = mon->to_json();
    }

    json species_json = json::array();
    for (const auto& [id, species]: all_species)
    {
        species_json.push_back(species->to_json());
    }

    json trainers_json;
    for (const auto& [name, trainer]: trainers)
    {
        trainers_json[trainer->name] = trainer->to_json();
    }

    json locations_json;
    for (const auto& [name, location]: npc_gifts)
    {
        locations_json[location->name] = location->to_json();
    }
    for (const auto& [name, location]: fly_unlocks)
    {
        locations_json[location->name] = location->to_json();
    }
    for (const auto& [name, location]: famechecker_rewards)
    {
        locations_json[location->name] = location->to_json();
    }
     for (const auto& [name, location]: dex_rewards)
     {
         locations_json[location->name] = location->to_json();
     }
    for (const auto& [name, location]: trainer_rewards)
    {
        locations_json[location->name] = location->to_json();
    }
    for (const auto& [name, location]: ball_items)
    {
        locations_json[location->name] = location->to_json();
    }
    for (const auto& [name, location]: hidden_items)
    {
        locations_json[location->name] = location->to_json();
    }
    for (const auto& [name, location]: badges)
    {
        locations_json[location->name] = location->to_json();
    }

    std::map<std::string, std::string> warp_destinations;
    for (const auto& warp: warps)
    {
        std::string destination = "";
        for (const auto& other_warp: warps)
        {
            if (other_warp == warp)
            {
                continue;
            }

            if (warp->connects_to(*other_warp))
            {
                destination = other_warp->encode();
                break;
            }
        }

        warp_destinations[warp->encode()] = destination;
    }

    json output_json = {
        { "comment", "DO NOT MODIFY. This file was auto-generated. Your changes will likely be overwritten." },
        { "rom_names", rom_names },
        { "maps", maps_json },
        { "starter_pokemon", starter_pokemon_json },
        { "misc_pokemon", misc_pokemon_json },
        { "legendary_pokemon", legendary_pokemon_json },
        { "misc_ram_addresses", misc_ram_addresses },
        { "misc_rom_addresses", misc_rom_addresses },
        { "locations", locations_json },
        { "warps", warp_destinations },
        { "species", species_json },
        { "trainers", trainers_json },
        { "tmhm_moves", tmhm_moves },
        { "constants", constants_json },
    };

    std::cout << "Writing file..." << std::endl;
    std::ofstream outfile(root_dir / out_file);
    outfile << output_json.dump() << std::endl;
}

json LocationInfo::to_json ()
{
    return {
        { "flag", this->flag },
        { "address", this->address },
        { "default_item", this->default_item }
    };
}

bool WarpInfo::connects_to (const WarpInfo &other)
{
    if (this->dest_map != other.source_map) return false;

    bool contains_all_indices = true;
    for (uint dest_i: this->dest_indices)
    {
        bool found_index = false;
        for (uint other_source_i: other.source_indices)
        {
            if (dest_i == other_source_i)
            {
                found_index = true;
                break;
            }
        }

        if (!found_index) {
            contains_all_indices = false;
            break;
        }
    }

    return contains_all_indices;
}

std::string WarpInfo::encode ()
{
    std::string result = "";

    result += this->source_map + ":";
    std::set<int> sorted_source_indices(this->source_indices.begin(), this->source_indices.end());
    for (int i: sorted_source_indices)
    {
        result += std::to_string(i) + ",";
    }
    result.pop_back();

    result += "/";

    result += this->dest_map + ":";
    std::set<int> sorted_dest_indices(this->dest_indices.begin(), this->dest_indices.end());
    for (int i: sorted_dest_indices)
    {
        result += std::to_string(i) + ",";
    }
    result.pop_back();

    if (this->is_one_way)
    {
        result += '!';
    }

    return result;
}

WarpInfo WarpInfo::decode (std::string s)
{
    bool is_one_way = false;
    if (s.back() == '!')
    {
        is_one_way = true;
        s.pop_back();
    }

    size_t slash_i = s.find('/');
    std::string source = s.substr(0, slash_i);
    std::string dest = s.substr(slash_i + 1);

    size_t source_colon_i = source.find(':');
    std::string source_map = source.substr(0, source_colon_i);
    std::string source_indices_string = source.substr(source_colon_i + 1);
    size_t dest_colon_i = dest.find(':');
    std::string dest_map = dest.substr(0, dest_colon_i);
    std::string dest_indices_string = dest.substr(dest_colon_i + 1);

    size_t i = 0;
    size_t prev_i = 0;
    std::vector<int> source_indices;
    while ((i = source_indices_string.find(',', i)) < source_indices_string.size())
    {
        source_indices.push_back(std::stoi(source_indices_string.substr(prev_i, i - prev_i)));
        prev_i = i + 1;
    }

    i = 0;
    prev_i = 0;
    std::vector<int> dest_indices;
    while ((i = dest_indices_string.find(',', i)) < dest_indices_string.size())
    {
        dest_indices.push_back(std::stoi(dest_indices_string.substr(prev_i, i - prev_i)));
        prev_i = i + 1;
    }

    WarpInfo warp_info;
    warp_info.is_one_way = is_one_way;
    warp_info.source_map = source_map;
    warp_info.source_indices = source_indices;
    warp_info.dest_map = dest_map;
    warp_info.dest_indices = dest_indices;

    return warp_info;
}

json MapInfo::to_json ()
{
    json map_json = {
        { "warp_table_address", this->warp_table_address },
        { "header_address", this->header_address },
    };

    if (this->land_encounters.exists)
    {
        map_json["land_encounters"] = this->land_encounters.to_json();
    }
    if (this->water_encounters.exists)
    {
        map_json["water_encounters"] = this->water_encounters.to_json();
    }
    if (this->fishing_encounters.exists)
    {
        map_json["fishing_encounters"] = this->fishing_encounters.to_json();
    }

    return map_json;
}

json EncounterTableInfo::to_json ()
{
    if (!this->exists) return nullptr;
    json slots_json;
    for (const auto &[version, encounter_slots]: this->encounter_slots)
    {
        for(const auto &encounter_slot: *encounter_slots)
        {
            slots_json[version].push_back(encounter_slot->to_json());
        }
    }

    return {
        { "slots", slots_json },
        { "address", this->address },
    };
}

json EncounterSlotInfo::to_json ()
{
    return {
        { "default_species", this->default_species },
        { "min_level", this->min_level },
        { "max_level", this->max_level }
    };
}

json StaticPokemonInfo::to_json ()
{
    return {
        { "species", this->species },
        { "address", this->address },
        { "level", this->level },
        { "level_address", this->level_address }
    };
}

json StarterPokemonInfo::to_json ()
{
    return {
        { "species", this->species },
        { "player_address", this->player_address },
        { "rival_address", this->rival_address }
    };
}

json SpeciesInfo::to_json ()
{
    const auto evolution_method_to_string = [](EvolutionMethod method) {
        switch (method)
        {
            case LEVEL:
                return "LEVEL";
            case LEVEL_ATK_LT_DEF:
                return "LEVEL_ATK_LT_DEF";
            case LEVEL_ATK_EQ_DEF:
                return "LEVEL_ATK_EQ_DEF";
            case LEVEL_ATK_GT_DEF:
                return "LEVEL_ATK_GT_DEF";
            case LEVEL_SILCOON:
                return "LEVEL_SILCOON";
            case LEVEL_CASCOON:
                return "LEVEL_CASCOON";
            case LEVEL_NINJASK:
                return "LEVEL_NINJASK";
            case LEVEL_SHEDINJA:
                return "LEVEL_SHEDINJA";
            case ITEM:
                return "ITEM";
            case FRIENDSHIP:
                return "FRIENDSHIP";
            default:
                throw new std::exception();
        }
    };


    json evolutions_json = json::array();
    for (const auto &evolution: this->evolutions)
    {
        evolutions_json.push_back({
            { "species", evolution.species },
            { "param", evolution.param },
            { "method", evolution_method_to_string(evolution.method) },
        });
    }

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(16) << std::hex << std::uppercase << this->tmhm_learnset;
    std::string tmhm_learnset_str = ss.str();

    return {
        { "address", this->address },
        { "id", this->id },
        { "base_stats", {
            this->base_stats[0],
            this->base_stats[1],
            this->base_stats[2],
            this->base_stats[3],
            this->base_stats[4],
            this->base_stats[5],
        } },
        { "types", {
            this->types[0],
            this->types[1],
        } },
        { "abilities", {
            this->abilities[0],
            this->abilities[1],
        } },
        { "catch_rate", this->catch_rate },
        { "friendship", this->friendship },
        { "learnset", this->learnset_info.to_json() },
        { "tmhm_learnset", tmhm_learnset_str },
        { "evolutions", evolutions_json },
    };
}

json LearnsetInfo::to_json ()
{
    json moves_json = json::array();
    for (const auto& move: this->moves)
    {
        moves_json.push_back({
            { "level", std::get<0>(move) },
            { "move_id", std::get<1>(move) },
        });
    }

    return {
        { "address", this->address },
        { "moves", moves_json },
    };
}

json TrainerInfo::to_json ()
{
    json party_json = json::array();
    for (const auto& pokemon: this->party)
    {
        json mon = {
            { "species", pokemon.species },
            { "level", pokemon.level },
        };

        if (this->pokemon_data_type == NO_ITEM_CUSTOM_MOVES || this->pokemon_data_type == ITEM_CUSTOM_MOVES)
        {
            mon["moves"] = pokemon.moves;
        }

        party_json.push_back(mon);
    }

    std::string pokemon_data_type_string;
    switch (this->pokemon_data_type)
    {
        case NO_ITEM_DEFAULT_MOVES:
            pokemon_data_type_string = "NO_ITEM_DEFAULT_MOVES";
            break;
        case NO_ITEM_CUSTOM_MOVES:
            pokemon_data_type_string = "NO_ITEM_CUSTOM_MOVES";
            break;
        case ITEM_DEFAULT_MOVES:
            pokemon_data_type_string = "ITEM_DEFAULT_MOVES";
            break;
        case ITEM_CUSTOM_MOVES:
            pokemon_data_type_string = "ITEM_CUSTOM_MOVES";
            break;
    }

    return {
        { "address", this->address },
        { "party_address", this->party_address },
        { "party", party_json },
        { "data_type", pokemon_data_type_string },
    };
}
