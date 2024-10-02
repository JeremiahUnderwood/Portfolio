/***
 * 
 * Created by Jeremiah Underwood
 * Last Edited 2/19/2024 by Jeremiah Underwood
 * 
 * 
***/
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;

public class FriendlyTask : MonoBehaviour, IStateTemplate
{

    public float _timeSinceStart = 0;
    [SerializeField] Vector2 _taskDurration; // min and max task durration
    private float _taskInstanceDurration; // task durration of current instance of task

    [field:SerializeField] public string Name { get; private set; } = "StateDefaultTask";
    public StateMachine Machine { get; set; }
    [field: SerializeField] public string AnimParam { get; private set; }

    public void StateEnter() 
    {
        _timeSinceStart = 0;
        _taskInstanceDurration = Random.Range(_taskDurration.x, _taskDurration.y);
        Machine.StopMovement();
        Machine.SetAnim(AnimParam, true);
    }

    public void StateExit() 
    {
        Machine.SetAnim(AnimParam, false);
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
        if (_timeSinceStart > _taskInstanceDurration)
        {
            return "NextTask";
        }
        return null;
    }

    public StateMachine SetStateMachine()
    {
        return Machine;
    }

    public void SetStateMachine(StateMachine newMachine)
    {
        Machine = newMachine;
    }
}
