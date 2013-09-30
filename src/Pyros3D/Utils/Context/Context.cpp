//============================================================================
// Name        : Context.cpp
// Author      : Duarte Peixinho
// Version     :
// Copyright   : ;)
// Description : Creates a Context
//============================================================================

#include "Context.h"

namespace p3d {

    
        Context::Context(const unsigned int &width, const unsigned int &height) : Width(width), Height(height), Initialized(true) {}
        
        Context::~Context() {}
       
        f64 Context::GetTime()
        {
            return deltaTime.GetTime();
        }
        
        f64 Context::GetTimeInterval()
        {
            return deltaTime.GetTimeInterval();
        }
        void Context::ActivateBulletTime(const f32& factor)
        {
            deltaTime.StartBulletTime(factor);
        }
        void Context::DeactivateBulletTime()
        {
            deltaTime.StopBulletTime();
        }
        void Context::SetTime(const f32 &Timer)
        {
            // Update Clock
            deltaTime.Update(Timer);
            
        }
        void Context::Pause()
        {
            deltaTime.Pause();
        }
        void Context::Resume()
        {
            deltaTime.Resume();
        }
    
        void Context::Close()
        {
            Initialized = false;
        }
}