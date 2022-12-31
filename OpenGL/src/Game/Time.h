﻿#pragma once

namespace Game
{
    enum TimeUnit
    {
        Second,
        MilliSecond,
        NanoSecond,
        Frame
    };
    
    class Time
    {
        int currentFrame = 0;
        float currentMs = 0;
        float deltaTime;
        float frameRate;

    public:
        Time();
        ~Time();

        float TotalDuration(TimeUnit unit = Second) const;
        float DeltaTime    (TimeUnit unit = Second) const;
        
        int TotalFrames() const { return currentFrame; }

        void Update();
    };
    
    extern Time Time;
}

