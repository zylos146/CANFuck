// Content of this file was generated

#ifndef GEN_HSM_TESTBASE
#define GEN_HSM_TESTBASE

#include <hsmcpp/hsm.hpp>

namespace testStates {
    constexpr hsmcpp::StateID_t Booting = 0;
    constexpr hsmcpp::StateID_t Initialization = 1;
    constexpr hsmcpp::StateID_t Logo = 2;
    constexpr hsmcpp::StateID_t Wifi = 3;
    constexpr hsmcpp::StateID_t ap_connection = 4;
    constexpr hsmcpp::StateID_t ap_host = 5;
}

namespace testEvents {
    constexpr hsmcpp::EventID_t connection_timeout = 0;
    constexpr hsmcpp::EventID_t ON_TIMER_LOGO_TIMER = 1;
    constexpr hsmcpp::EventID_t Finished = 2;

    // INVALID = INVALID_ID
}

namespace testTimers {
    constexpr hsmcpp::TimerID_t LOGO_TIMER = 0;
}

class testBase: public hsmcpp::HierarchicalStateMachine {
    using testBaseTransitionCallbackPtr_t           = void (testBase::*)(const hsmcpp::VariantVector_t&);
    using testBaseTransitionConditionCallbackPtr_t  = bool (testBase::*)(const hsmcpp::VariantVector_t&);
    using testBaseStateChangedCallbackPtr_t         = void (testBase::*)(const hsmcpp::VariantVector_t&);
    using testBaseStateEnterCallbackPtr_t           = bool (testBase::*)(const hsmcpp::VariantVector_t&);
    using testBaseStateExitCallbackPtr_t            = bool (testBase::*)();
    using testBaseTransitionFailedCallbackPtr_t     = void (testBase::*)(const hsmcpp::EventID_t, const hsmcpp::VariantVector_t&);

public:
    testBase();
    virtual ~testBase() = default;

    bool initialize(const std::shared_ptr<hsmcpp::IHsmEventDispatcher>& dispatcher) override;

protected:
    void configureHsm();
    void configureStates();
    void configureSubstates();
    void configureTransitions();
    void configureTimers();
    void configureActions();

// HSM state changed callbacks
protected:

// HSM state entering callbacks
protected:

// HSM state exiting callbacks
protected:

// HSM transition callbacks
protected:
    // NOTE: override this method in child class if needed
    virtual void onTransitionFailed(const hsmcpp::EventID_t event, const hsmcpp::VariantVector_t& args);


// HSM transition condition callbacks
protected:

protected:
    std::string getStateName(const hsmcpp::StateID_t state) const override;
    std::string getEventName(const hsmcpp::EventID_t event) const override;
};

#endif // GEN_HSM_TESTBASE