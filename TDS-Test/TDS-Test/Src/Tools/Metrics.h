#pragma once

#include <chrono>
#include <string>
#include <map>

class FuncTest
{
public:
	FuncTest();
	FuncTest(std::string name);

	void Log(double totalTime) const;

	void Start();
	void End();
	void Reset();

private:
	std::chrono::high_resolution_clock::time_point funcStartTime;
	std::chrono::high_resolution_clock::duration funcTime;
	unsigned int funcCalls;
	std::string name;
};

class Metric
{
public:
	Metric(std::string name, std::map<std::string, FuncTest> feelers);
	~Metric();

	void Loop();
	void LoopBegin();
	void LoopEnd();

	std::map<std::string, FuncTest> feelers;

	std::string GetName() const;

private:
	std::chrono::high_resolution_clock::time_point loopStartTime;
	std::chrono::high_resolution_clock::duration loopTime;
	
	bool firstRun;
	void Log() const;
	std::string name;
};

class Metrics {
private:
	static Metric sMetrics[];
public:
	static Metric& GetMetric(std::string name);
};