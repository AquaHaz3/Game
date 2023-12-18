from ctypes import*
import os

global _bridge

def addBlock(texture: str):

	args = _bridge.CreateArgs()
	_bridge.AddToArgs(args, texture.encode())
	_bridge.Execute("AddBlock".encode(), args)
	_bridge.DisposeArgs(args)

def InitBlocks():

	addBlock("bee_nest.png")
	addBlock("melon_top.png")
	addBlock("pumpkin_top.png")

def entry():

	print("[PYTHON] 'entry' : Init bridge")
	global _bridge
	path = os.getcwd();
	if ("scripts" in path) == False:
		path = os.path.dirname(os.getcwd()) + "\\scripts"
	_bridge = cdll.LoadLibrary(path+"\\PyBridgeDll.dll")
	print("[PYTHON] 'entry' : Start")
	InitBlocks()
	print("[PYTHON] 'entry' : End")