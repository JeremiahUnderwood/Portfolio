/***
 * 
 * Created by Jeremiah Underwood
 * Last Edited 1/24/2024 by Jeremiah Underwood
 * 
 * 
***/
using System.Collections;
using System.Collections.Generic;
using UnityEngine.AI;
using UnityEngine;

public class EnemyMove : MonoBehaviour, IStateTemplate
{

    [field:SerializeField] public string Name { get; private set; } = "StateIdle";
    public StateMachine Machine { get; set; }

    [field: SerializeField] public string AnimParam { get; private set; }

    [SerializeField] private Vector3 destination;


    /// THIS WHOLE CODE IS VERY SCUFFED
    public void StateEnter() 
    {
        Machine.SetDestination(destination);
    }

    public void StateExit() 
    {
        
    }

    public void StateUpdate() 
    {
        
    }

    public void StatePhysicsUpdate() 
    {
        
    }

    public string ExitConditions() 
    {
        if ((Machine.GetLocation() - destination).magnitude < 3)
        {
            return "EnemyIdle";
        }
        return null;
    }

    public void SetStateMachine(StateMachine newMachine)
    {
        Machine = newMachine;
    }
}
