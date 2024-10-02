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

public class EnemyTarget : MonoBehaviour, IStateTemplate
{

    [field:SerializeField] public string Name { get; private set; } = "EnemyTarget";
    public StateMachine Machine { get; set; }

    [field: SerializeField] public string AnimParam { get; private set; }

    [SerializeField] private Transform target;
    [SerializeField] private float attackRange = 5;


    /// THIS WHOLE CODE IS VERY SCUFFED
    public void StateEnter() 
    {

    }

    public void StateExit() 
    {
        Machine.StopMovement();
    }

    public void StateUpdate() 
    {
        
    }

    public void StatePhysicsUpdate() 
    {
        Machine.SetDestination(target.position);
    }

    public string ExitConditions() 
    {
        if ((Machine.GetLocation() - target.position).magnitude < attackRange)
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
