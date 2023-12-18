#pragma once

#include <string>
#include <Python.h>
#include <map>

struct PyFunc {

	PyFunc();
	PyFunc(PyObject* module, std::string name, int argc);
	~PyFunc();

	void call(PyObject* arg0, ...);
	void callStrings(const char* arg0, ...);
	void callVoid();

private:

	PyObject* func = 0;
	bool isInit = false;
	int argc = 0;

};

class PyFile {

public:

	PyFile(std::string filename);
	~PyFile();
	
	void addMethod(std::string name, int argc);

	std::map<std::string, PyFunc> methods;

private:

	std::string filename;

	PyObject* module;

};

class PyRuntime {

public:

	static void InitIfItNeeded();
	static void Dispose();

private:

	static bool isInit;

};