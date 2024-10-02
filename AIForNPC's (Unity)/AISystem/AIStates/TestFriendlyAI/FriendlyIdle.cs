using System.Collections;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;

public class FriendlyIdle : MonoBehaviour, IStateTemplate
{

    public float _timeSinceStart = 0;

    [field:SerializeField] public string Name { get; private set; } = "StateIdle";
    public StateMachine Machine { get; set; }

    [field: SerializeField] public string AnimParam { get; private set; }

    public void StateEnter() 
    {
        _timeSinceStart = 0;
    }

    public void StateExit() 
    {
        
    }

    public void StateUpdate() 
    {
        _timeSinceStart += Time.deltaTime;
    }

    public void StatePhysicsUpdate() 
    {
        
    }

    public string ExitConditions() 
    {
        if (_timeSinceStart > 10f)
        {
            return "FriendlyMove";
        }
        return null;
    }

    public void SetStateMachine(StateMachine newMachine)
    {
        Machine = newMachine;
    }
}
