#pragma once
#include<Windows.h>
class GameTimer
{
public:
	GameTimer():mSecondsPerCount(0.0),mDeltaTime(-1.0),mBaseTime(0),
		mPausedTime(0),mPrevTime(0),mCurrTime(0),mStopped(false)
	{
		__int64 countsPerSec;
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
		mSecondsPerCount = 1.0 / (double)countsPerSec;
	};
	~GameTimer() {};
	float GameTime()const;//in seconds
	float DeltaTime()const//in seconds
	{
		return (float)mDeltaTime;
	}
	void Reset()//在消息循环之前调用
	{
		__int64 currtime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currtime);
		mBaseTime = currtime;
		mPrevTime = currtime;
		mStopTime = 0;
		mStopped = false;
	}
	void Tick() //每个帧调用一次
	{
		if (mStopped)
		{
			mDeltaTime = 0.0;
			return;
		}
		__int64	currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		mCurrTime = currTime;
		//计算间隔时间
		mDeltaTime = (mCurrTime - mPrevTime)*mSecondsPerCount;
		//为下一次计算做准备
		mPrevTime = mCurrTime;
		if (mDeltaTime < 0.0)
		{
			mDeltaTime = 0.0;
		}
	}
	float TotalTime()const
	{
		if (mStopped)
		{
			return (float)(((mStopTime - mPausedTime) - mBaseTime)*mSecondsPerCount);
		}

	
		else
		{
			return (float)(((mCurrTime - mPausedTime) - mBaseTime)*mSecondsPerCount);
		}
	}
	void Start()//取消暂停时调用
	{
		__int64 startTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&startTime);


	
		if (mStopped)
		{
			mPausedTime += (startTime - mStopTime);

			mPrevTime = startTime;
			mStopTime = 0;
			mStopped = false;
		}
	}
	void Stop()//暂停时调用
	{
		if (!mStopped)
		{
			__int64 currTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

			mStopTime = currTime;
			mStopped = true;
		}
	}
private:
	double mSecondsPerCount;
	double mDeltaTime;
	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;
	bool mStopped;

	
};