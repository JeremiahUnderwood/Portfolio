using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StateIdle : MonoBehaviour, IStateTemplate
{

    [field:SerializeField] public string Name { get; private set; } = "StateIdle";
    public StateMachine Machine { get; set;  }

    [field: SerializeField] public string AnimParam { get; private set; }

    public void StateEnter() 
    {
        Debug.Log("Enter");
    }

    public void StateExit() 
    {
        Debug.Log("Exit");
    }

    public void StateUpdate() 
    {
        Debug.Log("Update");
    }

    public void StatePhysicsUpdate() 
    {
        Debug.Log("PhysUpdate");
    }

    public string ExitConditions() 
    {
        return null;
    }

    public void SetStateMachine(StateMachine newMachine)
    {
        Machine = newMachine;
    }
}
