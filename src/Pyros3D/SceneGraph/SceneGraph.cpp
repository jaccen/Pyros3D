//============================================================================
// Name        : SceneGraph.cpp
// Author      : Duarte Peixinho
// Version     :
// Copyright   : ;)
// Description : SceneGraph
//============================================================================

#include "SceneGraph.h"
#include "../Utils/Thread/Thread.h"
#include <string.h>

namespace p3d {
    
    SceneGraph::SceneGraph() 
    {
        echo("SUCCESS: Scene Created");
    }
    
    void SceneGraph::Add(GameObject* GO)
    {
        if (GO->Scene==NULL)
        {
            bool found = false;
            for (std::vector<GameObject*>::iterator i=_GameObjectList.begin();i!=_GameObjectList.end();i++)
            {
                if (*i==GO)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                _GameObjectList.push_back(GO);
                // Set Scene Pointer
                GO->Scene = this;
                echo("SUCCESS: GameObject Added to Scene");
            } else {
                echo("ERROR: Component Already Added in the Scene");
            }
        } else {
            echo("ERROR: GameObject Already on a Scene");
        }
    }
    
    void SceneGraph::Remove(GameObject* GO)
    {
        bool found = false;
        for (std::vector<GameObject*>::iterator i=_GameObjectList.begin();i!=_GameObjectList.end();i++)
        {
            if (*i==GO)
            {
                // Unregister Components
                (*i)->UnregisterComponents(this);
                // Erase From List
                _GameObjectList.erase(i);
                // Erase Scene Pointer
                GO->Scene = NULL;
                // Set Flag
                found = true;
                break;
            }
        }
        if (!found) echo("GameObject Not Found in Scene");
        else echo("SUCCESS: GameObject Removed from Scene");
    }
    
    void SceneGraph::Update(const f64 &Timer)
    {
        // Save Time
        timer = Timer;
        
        for (std::vector<GameObject*>::iterator i=_GameObjectList.begin();i!=_GameObjectList.end();i++)
        {
            // Update GameObject - User Change
            (*i)->Update();
            // Register Components
            (*i)->RegisterComponents(this);
            // Update Components
            (*i)->UpdateComponents();
            
#ifndef MULTI_THREAD
            
            // NOT MULTI THREADED
            
            // Update Transforms Not Using Threads
            (*i)->InternalUpdate();
            (*i)->CloneTransform();
            
        }
#else
        // MULTI THREADED
        
        if (!_ThreadIsUpdating && !_ThreadSync)
        {
            // Copy GameObjects to Thread
            _GameObjectList.resize(_GameObjectList.size());
            
            // Register Thread
            ThreadID = Thread::AddThread(UpdateTransformations);
                    
        } else {
            if (_ThreadIsUpdating && !_ThreadSync)
            {
                // Remove Thread
                Thread::RemoveThread(ThreadID);

                // Copy From Thread to GameObjects
                _ThreadSync = true;
                for (std::vector<GameObject*>::iterator i=_GameObjectList.begin();i!=_GameObjectList.end();i++)
                {
                    // Copy from Thread
                    (*i)->CloneTransform();
                }
                _ThreadSync = false;
            }
        }
#endif

    }
  
    // Thread Function
    void* SceneGraph::UpdateTransformations(SceneGraph* Scene)
    {
        // Set Flag
        Scene->_ThreadIsUpdating = true;
        for (std::vector<GameObject*>::iterator i=Scene->_GameObjectList.begin();i!=Scene->_GameObjectList.end();i++)
        {
            (*i)->InternalUpdate();
        }
        // Unset Flag
        Scene->_ThreadIsUpdating = false;

        return NULL;
    }
    
    const f64 &SceneGraph::GetTime() const 
    {
        return timer;
    }
    
};