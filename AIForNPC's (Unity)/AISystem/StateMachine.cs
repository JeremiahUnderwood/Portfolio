/***
 * 
 * Created by Jeremiah Underwood
 * Last Edited 2/19/2024 by Jeremiah Underwood
 * State machine that gathers states from child objects and manages overhead for state transitions, includes references those states might need.
 * 
***/
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StateMachine : MonoBehaviour
{
    private Dictionary<string, IStateTemplate> _states = new Dictionary<string, IStateTemplate>(); // dictionary containing each state referenced by its state name
    [SerializeField] private string m_defaultState; //default state unique to instance
    [SerializeField] private NPCBehavior _behavior;
    private IStateTemplate _currentState;
    [SerializeField] private string _currentStateName;
    private Vector3 _destination;

    // Start is called before the first frame update
    void Start()
    {
        foreach (IStateTemplate script in GetComponentsInChildren<IStateTemplate>()){
            _states.Add(script.Name, script);
            Debug.Log(script.Name);
        }
        if (_states.ContainsKey(m_defaultState)) 
        {
            _currentState = _states[m_defaultState];
            _currentStateName = m_defaultState;
            _currentState.SetStateMachine(this);
            _currentState.StateEnter();
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (_currentState != null){
            _currentState.StateUpdate();
            if (_currentState.ExitConditions() != null)
            {
                IStateTemplate prevState = _currentState;
                _currentStateName = _currentState.ExitConditions();
                _currentState = _states[_currentState.ExitConditions()];
                prevState.StateExit(); //this is done with a new variable to ensure that the StateExit function doesn't interfere with the ExitConditions function
                _currentState.SetStateMachine(this);
                _currentState.StateEnter();

            }
        }
    }

    void FixedUpdate()
    {
        if (_currentState != null)
        {
            _currentState.StatePhysicsUpdate();
        }
    }

    // GETTERS AND SETTERS //
    public NPCBehavior GetBehavior()
    {
        return _behavior;
    }

    public void SetBehavior(NPCBehavior newBehavior)
    {
        _behavior = newBehavior;
    }

    public string GetStateName()
    {
        return _currentStateName; 
    }

    public void SetDestination(Vector3 newDest)
    {
        _destination = newDest;
        _behavior.SetAgentDestination(_destination);
    }

    public void StopMovement()
    {
        SetDestination(GetLocation());
    }
    
    public Vector3 GetDestination()
    {
        return _destination;
    }

    public Vector3 GetLocation()
    {
        return GetComponentInParent<Transform>().position;
    }

    public void SetAnim(string param, bool mode)
    {
        _behavior.SetAnimation(param, mode);
    }

}
