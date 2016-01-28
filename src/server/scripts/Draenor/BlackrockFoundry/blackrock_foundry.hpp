////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef BLACKROCK_FOUNDRY_HPP
# define BLACKROCK_FOUNDRY_HPP

# include "InstanceScript.h"
# include "ScriptedCosmeticAI.hpp"
# include "ScriptedCreature.h"
# include "ScriptMgr.h"
# include "ScriptUtils.h"
# include "Cell.h"
# include "CellImpl.h"
# include "GridNotifiers.h"
# include "GridNotifiersImpl.h"
# include "Vehicle.h"
# include "GameObjectAI.h"
# include "Group.h"
# include "MoveSplineInit.h"
# include "CreatureTextMgr.h"

float const g_NorthOrientation = 0.0f;
float const g_SouthOrientation = M_PI;
float const g_WestOrientation = M_PI / 2.0f;
float const g_EastOrientation = 3.0f * M_PI / 2.0f;

static void CastSpellToPlayers(Map* p_Map, Unit* p_Caster, uint32 p_SpellID, bool p_Triggered)
{
    if (p_Map == nullptr)
        return;

    Map::PlayerList const& l_Players = p_Map->GetPlayers();
    for (Map::PlayerList::const_iterator l_Iter = l_Players.begin(); l_Iter != l_Players.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
        {
            if (p_Caster != nullptr)
                p_Caster->CastSpell(l_Player, p_SpellID, p_Triggered);
            else
                l_Player->CastSpell(l_Player, p_SpellID, p_Triggered);
        }
    }
}

static void DespawnCreaturesInArea(uint32 p_Entry, WorldObject* p_WorldObject)
{
    std::list<Creature*> l_Creatures;
    GetCreatureListWithEntryInGrid(l_Creatures, p_WorldObject, p_Entry, p_WorldObject->GetMap()->GetVisibilityRange());

    for (Creature* l_Iter : l_Creatures)
        l_Iter->DespawnOrUnsummon();
}

enum eFoundryCreatures
{
    /// Slagworks - Part 1
    /// Gruul
    BossGruul               = 76877,
    /// Oregorger
    BossOregorger           = 77182,
    /// Blast Furnace
    HeartOfTheMountain      = 76806,
    ForemanFeldspar         = 76809,
    BlackhandCosmetic       = 76831,
    /// The Black Forge - Part 2
    /// Hans'gar & Franzok
    BossHansgar             = 76973,
    BossFranzok             = 76974,
    /// Flamebender Ka'graz
    BossFlamebenderKagraz   = 76814,
    AknorSteelbringer       = 77337,
    LavaStalker             = 77043,
    MoltenTorrentStalker    = 77253,
    CinderWolf              = 76794,
    /// Kromog
    BossKromog              = 77692,
    /// Iron Assembly - Part 3
    /// Beastlord Darmac
    BossBeastlordDarmac     = 76865,
    /// Operator Thogar
    BossOperatorThogar      = 76906,
    /// Iron Maidens
    BossAdmiralGaran        = 77557,
    BossEnforcerSorka       = 77231,
    BossMarakTheBlooded     = 77477,
    /// Blackhand's Crucible - Part 4
    /// Blackhand
    BossBlackhand           = 77325
};

enum eFoundryGameObjects
{
    /// Slagworks - Part 1
    SlagworksDoor               = 238835,
    GruulSpikeDoor              = 230930,
    BKFoundrySpikeTrapGate      = 230931,
    FurnacePortcullis           = 237224,
    BlastFurnaceEncounterDoor   = 230759,
    CrucibleLeft                = 233759,
    CrucibleRight               = 233839,
    FurnaceGate                 = 227423,
    /// The Black Forge - Part 2
    BlackForgePortcullis        = 238836,
    VolatileBlackrockOre        = 237308,
    HansgarAndFranzokEntrance   = 229318,
    HansgarAndFranzokExit       = 229319,
    ConveyorBelt002             = 230481,
    ConveyorBelt001             = 230482,
    ConveyorBelt003             = 230483,
    ConveyorBelt004             = 230484,
    ConveyorBelt005             = 230485,
    StampingPress16             = 229574,
    StampingPress20             = 229575,
    StampingPress19             = 229576,
    StampingPress18             = 229577,
    StampingPress17             = 229578,
    StampingPress15             = 229579,
    StampingPress14             = 229580,
    StampingPress13             = 229581,
    StampingPress12             = 229582,
    StampingPress11             = 229583,
    StampingPress06             = 229584,
    StampingPress07             = 229585,
    StampingPress08             = 229586,
    StampingPress09             = 229587,
    StampingPress10             = 229588,
    StampingPress05             = 229589,
    StampingPress04             = 229590,
    StampingPress01             = 229591,
    StampingPress02             = 229592,
    StampingPress03             = 229593,
    ConveyorBelt006             = 233238,
    ConveyorBelt007             = 233239,
    ConveyorBelt008             = 233240,
    ConveyorBelt009             = 236843,
    ConveyorBelt010             = 236844,
    FirewallDoor                = 236845,
    /// Iron Assembly - Part 3
    IronAssembleyGate           = 238837,
    /// Blackhand's Crucible - Part 4
    SpikeGate                   = 232556,
    CrucibleDoor                = 233006
};

enum eFoundryDatas
{
    /// Bosses
    DataGruul,
    DataOregorger,
    DataBlastFurnace,
    DataHansgarAndFranzok,
    DataFlamebenderKagraz,
    DataKromog,
    DataBeastlordDarmac,
    DataOperatorThogar,
    DataIronMaidens,
    DataBlackhand,
    MaxBossData,

    /// Other boss datas
    DataForemanFeldspar     = eFoundryDatas::MaxBossData,

    /// Misc
    PristineTrueIronOres    = 0,
    VolatileOreGrinded,
    PrimalElementalistTime,
    PlayerStamped,
    IronTaskmasterAggro,
    AknorDied,

    /// Counters
    /// Gruul
    MaxPristineTrueIronOres = 3,
    /// Oregorger
    MaxOreCrateSpawns       = 25,
    MaxOregorgerMovePos     = 8,
    MaxOregorgerPatterns    = 7,
    MaxOregorgerCollisions  = 8,
    MaxOregorgerPaths       = 12,
    /// Blast Furnace
    MaxPrimalElementalists  = 4
};

enum eFoundrySpells
{
    Berserker = 26662
};

enum eFoundryAchievements
{
    TheIronPrince           = 8978,
    HeShootsHeOres          = 8979,
    YaWeveGotTime           = 8930,
    StampStampRevolution    = 8980,
    TheSteelHasBeenBrought  = 8929
};

enum eFoundryVisuals
{
    CrucibleVisuals = 6922,
    ConveyorsStart  = 7621,
    ConveyorsStop   = 7336,
    ConveyorsStart2 = 6975,
    ConveyorsStart3 = 6974
};

#endif