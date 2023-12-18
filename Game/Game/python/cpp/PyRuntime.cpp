#include "PyRuntime.hpp"

PyFile::PyFile(std::string filename)
{
	PyRuntime::InitIfItNeeded();
	this->filename = filename;

    PyObject* pName;

    pName = PyUnicode_DecodeFSDefault(filename.c_str());

    PyRun_SimpleString("import sys\nimport os\n"
        "sys.path.append(os.path.dirname(os.path.abspath(os.getcwd()))+'/scripts/')\n"
    );

    module = PyImport_Import(pName);

	Py_DECREF(pName);
}

PyFile::~PyFile()
{
    if (module != 0) {
        Py_DECREF(module);
    }
}

void PyFile::addMethod(std::string name, int argc)
{
	methods.emplace(name, PyFunc(module, name, argc));
}

bool PyRuntime::isInit = false;

void PyRuntime::InitIfItNeeded()
{
	if (isInit) return;
	Py_Initialize();
	isInit = true;
}

void PyRuntime::Dispose()
{
	if (!isInit) return;
	Py_Finalize();
	isInit = false;
}

PyFunc::PyFunc() {}

PyFunc::PyFunc(PyObject* module, std::string name, int argc)
{
	func = PyObject_GetAttrString(module, name.c_str());
	isInit = (func && PyCallable_Check(func));
	this->argc = argc;
}

PyFunc::~PyFunc()
{
	if (func != 0) Py_DECREF(func);
}

void PyFunc::call(PyObject* arg0, ...)
{
	if (!isInit) return;

	if (argc <= 0) {
		callVoid(); return;
	}

    va_list args;
    va_start(args, arg0);

	auto pArgs = PyTuple_New(argc);
	PyTuple_SetItem(pArgs, 0, arg0);
    for(int i = 1; i < argc; i++)
    {
		PyObject* obj = va_arg(args, PyObject*);
		PyTuple_SetItem(pArgs, i, obj);
    }
    va_end(args);

	PyObject_CallObject(func, pArgs);
	Py_DECREF(pArgs);
}

void PyFunc::callStrings(const char* arg0, ...)
{
	if (!isInit) return;

	if (argc <= 0) {
		callVoid(); return;
	}

	va_list args;
	va_start(args, arg0);

	auto pArgs = PyTuple_New(argc);
	PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(arg0));
	for (int i = 1; i < argc; i++)
	{
		const char* obj = va_arg(args, const char*);
		PyTuple_SetItem(pArgs, i, PyUnicode_FromString(obj));
	}
	va_end(args);

	PyObject_CallObject(func, pArgs);
	Py_DECREF(pArgs);
}

void PyFunc::callVoid()
{
	if (!isInit) return;
	auto pArgs = PyTuple_New(0);
	PyObject_CallObject(func, pArgs);
	if (PyErr_Occurred())
		PyErr_Print();
	Py_DECREF(pArgs);
}
