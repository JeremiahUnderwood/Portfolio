/***
 * 
 * Created By Jeremiah Underwood
 * Laste Edited 1/24/2024 by Jeremiah Underwood
 * IF THIS IS NOT IN INTERFACES IN THE UNSTABLE TEST BRANCH, MOVE IT THERE!!!
 * 
***/


using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public interface IStateTemplate
{

    StateMachine Machine { get; }
    string Name { get; }
    string AnimParam { get; }

    void StateEnter();

    void StateExit();

    void StateUpdate();

    void StatePhysicsUpdate();

    string ExitConditions();

    void SetStateMachine(StateMachine newMachine);
}
