/***
 * 
 * Created by Jeremiah Underwood
 * Last Edited 2/19/2024 by Jeremiah Underwood
 * Script that handles unique behaviors to the NPC
 * 
***/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;
using UnityEngine.Animations;

public class NPCBehavior : MonoBehaviour
{
    protected NavMeshAgent _agent;
    [SerializeField] protected StateMachine _machine;
    protected Animator m_animationController;

    //These need to be set before the state machine calls its start function
    private void Awake()
    {
        _agent = GetComponent<NavMeshAgent>();
        if (_machine != null) _machine = GetComponentInChildren<StateMachine>();
        if (_machine.GetBehavior() != null) _machine.SetBehavior(this);

        m_animationController = GetComponent<Animator>();
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void PhysicsUpdate()
    {

    }

    public void SetAgentDestination(Vector3 destination)
    {
        _agent.SetDestination(destination);
    }

    public void SetAnimation(string param, bool mode)
    {
        m_animationController.SetBool(param, mode);
    }
}
