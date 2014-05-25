/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	trainingmission.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "trainingmission" interface.
**
**  History:
*/
#ifndef _TRAINING_MISSION_H_
#define _TRAINING_MISSION_H_

#ifndef _GOAL_H_
#include "Goal.h"
#endif  //_GOAL_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class TrainingMission
    {
        public:
                    /* void */              TrainingMission (void);
            virtual /* void */              ~TrainingMission (void);
            virtual int                     GetMissionID (void) = 0;
            virtual SectorID                GetStartSectorID (void) = 0;
            virtual void                    SetupShipAndCamera (void);
                    void                    Start (void);
                    bool                    Frame (void);
                    void                    Terminate (void);
                    void                    AddWaitCondition (Condition* pWaitCondition);
            virtual bool                    RecordKeyPress (TrekKey key);
                    void                    AddKeyCondition (Condition* pKeyCondition);
                    void                    RecordChat (ChatTarget recipient);
                    void                    SetChatCondition (Condition* pChatCondition);
            virtual bool                    ShipLanded (void);
            virtual void                    ShipDied (ImodelIGC* pLauncher);
            virtual bool                    RestoreShip (void);
                    IshipIGC*               GetCommanderShip (void) const;
                    void                    AddPartToShip (IshipIGC* pShip, PartID part, Mount mount, short ammo = 0);
                    void                    AddPartToShip (PartID part, Mount mount, short ammo = 0);
                    IshipIGC*               CreateDrone (const ZString& name, ShipID shipID, HullID hullID, SideID sideID, PilotType pilotType);
                    void                    SetSkipPostSlideshow (void);

        protected:
            virtual void                    CreateUniverse (void) = 0;
            virtual Condition*              CreateMission (void) = 0;
                    void                    LoadUniverse (const ZString& name, HullID hullID, StationID homeStationID);
                    void                    DefaultLoadout (IshipIGC* pShip);
                    void                    CreatePlayerInfo (IshipIGC* pShip, SideID side, ShipID shipID);

                    Goal*                   CreatePlaySoundGoal (SoundID sound);
                    Condition*              CreateTooCloseCondition (TRef<ImodelIGC> place);
                    Condition*              CreateTooLongCondition (float fDelay, SoundID sound);

        protected:
                    Condition*              m_pCondition;
                    Condition*              m_pDeadCondition;
                    std::list<Condition*>   m_WaitConditionList;
                    std::list<Condition*>   m_KeyConditionList;
                    Condition*              m_pChatCondition;
                    bool                    m_bMusicWasOn;
                    int                     m_fHUDStyle;
                    ObjectID                m_commanderID;
                    SectorID                m_deadSectorID;
                    TrekWindow::ViewMode    m_deadViewMode;
                    TrekWindow::CameraMode  m_deadCameraMode;
                    bool                    m_bSkipPostSlideShow;
    };

    //------------------------------------------------------------------------------
}

#endif  //_TRAINING_MISSION_H_
