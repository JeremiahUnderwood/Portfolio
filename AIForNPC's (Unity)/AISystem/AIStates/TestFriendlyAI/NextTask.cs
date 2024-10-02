/***
 * 
 * Created by Jeremiah Underwood
 * Last Edited 2/19/2024 by Jeremiah Underwood
 * 
 * 
***/
using System.Collections;
using System.Collections.Generic;
using UnityEngine.AI;
using UnityEngine;
using UnityEngine.Animations;
using System;
using UnityEngine.Apple;

public class NextTask: MonoBehaviour, IStateTemplate
{

    [field:SerializeField] public string Name { get; private set; } = "NextTask";
    public StateMachine Machine { get; set; }
    [field:SerializeField] public string AnimParam { get; private set; }

    [Serializable] public struct Task
    {
        public Vector3 Location;
        public string TaskState;
    }

    [SerializeField] private Task[] m_tasks;
    private int _currentTask = 0; //Index of current task when put through the m_tasks array



    
    public void StateEnter() 
    {
        Machine.SetDestination(m_tasks[_currentTask].Location);
        Machine.SetAnim(AnimParam, true);
    }

    public void StateExit() 
    {
        _currentTask++;
        if (_currentTask >= m_tasks.Length)
        {
            _currentTask = 0;
        }
        Machine.SetAnim(AnimParam, false);
    }

    public void StateUpdate() 
    {
        
    }

    public void StatePhysicsUpdate() 
    {
        
    }

    public string ExitConditions() 
    {
        if ((Machine.GetLocation() - m_tasks[_currentTask].Location).magnitude < 3)
        {
            //Debug.Log(m_tasks[_currentTask].TaskState);
            return m_tasks[_currentTask].TaskState;
        }
        return null;
    }

    public void SetStateMachine(StateMachine newMachine)
    {
        Machine = newMachine;
    }
}
