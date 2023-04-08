// Content of this file was generated

#include "testBase.hpp"

testBase::testBase()
    : hsmcpp::HierarchicalStateMachine(testStates::) {
}

bool testBase::initialize(const std::shared_ptr<hsmcpp::IHsmEventDispatcher>& dispatcher) {
    configureHsm();
    return hsmcpp::HierarchicalStateMachine::initialize(dispatcher);
}

void testBase::configureHsm() {
    registerFailedTransitionCallback<testBase>(this, &testBase::onTransitionFailed);
    configureStates();
    configureSubstates();
    configureTransitions();
    configureTimers();
    configureActions();
}

void testBase::configureStates() {
    registerState<testBase>(testStates::Booting);
    registerState<testBase>(testStates::Initialization);
    registerState<testBase>(testStates::Logo);
    registerState<testBase>(testStates::Wifi);
    registerState<testBase>(testStates::ap_connection);
    registerState<testBase>(testStates::ap_host);
}

void testBase::configureSubstates() {
    (void)registerSubstateEntryPoint(testStates::Booting, testStates::Initialization);
    (void)registerSubstate(testStates::Booting, testStates::Logo);
    (void)registerSubstate(testStates::Booting, testStates::Wifi);
    (void)registerSubstateEntryPoint(testStates::Wifi, testStates::ap_connection);
    (void)registerSubstate(testStates::Wifi, testStates::ap_host);
}

void testBase::configureTransitions() {
    registerTransition(testStates::Initialization, testStates::Logo, testEvents::Finished);
    registerTransition(testStates::Logo, testStates::Wifi, testEvents::ON_TIMER_LOGO_TIMER);
    registerTransition(testStates::ap_connection, testStates::ap_host, testEvents::connection_timeout);
}

void testBase::configureTimers() {
    registerTimer(static_cast<hsmcpp::TimerID_t>(testTimers::LOGO_TIMER), testEvents::ON_TIMER_LOGO_TIMER);
}

void testBase::configureActions() {
    (void)registerStateAction(testStates::Logo, StateActionTrigger::ON_STATE_ENTRY, StateAction::START_TIMER, static_cast<hsmcpp::TimerID_t>(testTimers::LOGO_TIMER), 2000, true);
}

void testBase::onTransitionFailed(const hsmcpp::EventID_t event, const hsmcpp::VariantVector_t& args) {
    // do nothing
}

std::string testBase::getStateName(const hsmcpp::StateID_t state) const {
    std::string stateName;

#ifndef HSM_DISABLE_TRACES
    switch(state) {
        case testStates::Booting:
            (void)stateName.assign("Booting");
            break;
        case testStates::Initialization:
            (void)stateName.assign("Initialization");
            break;
        case testStates::Logo:
            (void)stateName.assign("Logo");
            break;
        case testStates::Wifi:
            (void)stateName.assign("Wifi");
            break;
        case testStates::ap_connection:
            (void)stateName.assign("ap_connection");
            break;
        case testStates::ap_host:
            (void)stateName.assign("ap_host");
            break;
        default:
            stateName = hsmcpp::HierarchicalStateMachine::getStateName(state);
            break;
    }
#else
    stateName = hsmcpp::HierarchicalStateMachine::getStateName(state);
#endif // HSM_DISABLE_TRACES

    return stateName;
}

std::string testBase::getEventName(const hsmcpp::EventID_t event) const {
    std::string eventName;

#ifndef HSM_DISABLE_TRACES
    switch(event) {
        case testEvents::connection_timeout:
            (void)eventName.assign("connection_timeout");
            break;
        case testEvents::ON_TIMER_LOGO_TIMER:
            (void)eventName.assign("ON_TIMER_LOGO_TIMER");
            break;
        case testEvents::Finished:
            (void)eventName.assign("Finished");
            break;
        default:
            eventName = hsmcpp::HierarchicalStateMachine::getEventName(event);
            break;
    }
#else
    eventName = hsmcpp::HierarchicalStateMachine::getEventName(event);
#endif // HSM_DISABLE_TRACES

    return eventName;
}