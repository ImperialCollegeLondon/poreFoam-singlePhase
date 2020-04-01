#!/usr/bin/env python3

# This code is part of sindopy.
# sindopy is a Python interface to various models
# developed by Ali Q. Raeini (2019).


# LS_COLORS=$LS_COLORS:'di=1;36:ln=35' ; export LS_COLORS;
# export PYTHONPATH=$HOME/soft/src/script/pylib:$PYTHONPATH



# python initializer for launching porefoam and network modelling apps
import numpy
from numpy import sqrt,prod,exp,log,dot,multiply,inf

#try:
#from scipy.linalg import schur as _schur
#except ImportError:
#pass
import numpy as np
import math
from math import exp, expm1, floor
import os,sys,copy,time, subprocess
from sys import stdin,stdout,stderr
import unittest
# from scipy.special import gamma
from numpy import rint as fix
from numpy import genfromtxt
#from pylab import *
#import pandas as pd
import pdb
import matplotlib.pyplot  as plt
# from matplotlib.backends.backend_pdf import PdfPages
from matplotlib import rc
from matplotlib import ticker
import re
import io
import inspect

import functools
disp = functools.partial(print, flush=True)




try:    disp(msEnv['msSrc'])
except: msEnv = os.environ.copy(); pass



######################  SET SCRIPT DIRECTORIES    ######################


if "msSrc" in msEnv: 
	disp("Info: not resetting msSrc: "+msEnv['msSrc'])
	msSrc= msEnv['msSrc'];
else :
	msSrc = os.path.dirname(os.path.dirname(os.path.abspath(__file__))) # script directory
	disp(("Setting msSrc:",msSrc))
	sep=os.pathsep;
	msRoot = os.path.abspath(msSrc+"/..")
	msEnv["PATH"] = msEnv["PATH"] +sep+ msSrc+"/script"
	msEnv["PATH"] = msEnv["PATH"] +sep+ msSrc+"/exp/script" #EXP:;
	msEnv["PATH"] = msEnv["PATH"] +sep+ msSrc+"/porefoam1f/script"
	msEnv["PATH"] = msEnv["PATH"] +sep+ msSrc+"/porefoam2f/script"
	msEnv["PATH"] = msEnv["PATH"] +sep+ msSrc+"/gnm/script/PNM"
	try:msEnv["LD_LIBRARY_PATH"] = msEnv["LD_LIBRARY_PATH"] +sep+msRoot+"/lib"
	except:msEnv["LD_LIBRARY_PATH"] = msRoot+"/lib"
	msEnv["PATH"] = msEnv["PATH"] +sep+ msRoot+"/bin"
	sys.path.append(msSrc+"/pylib")
	msEnv.update({'msSrc':msSrc})
	os.environ.update({'msSrc':msSrc})
	os.environ.update({"PYTHONPATH", msSrc+"/pylib"+sep+os.environ.get("PYTHONPATH","")});





######################  BASIC TEST UTILITIES  ##########################


def DbgMsg(message='', nTrace=3, isError=1, endl='\n'):
	if isError :
		stak = inspect.stack()
		if nTrace==0:
			er = inspect.getframeinfo(stak[1][0]) # 0 represents this line, 1 represents line at caller
			disp(er.filename+':'+str(er.lineno)+':0, '+er.function+',  '+message,endl)
		else:
			for ii in range(0,min(nTrace,len(stak)-1)):
				er = inspect.getframeinfo(stak[ii+1][0]) # 0 represents this line, 1 represents line at caller
				disp(er.filename+':'+str(er.lineno)+':0, '+er.function,endl)
			disp(message,endl)
		# if isError>1 : raise # ValueError(message) #RuntimeError
		return -1
	return 0


def runBashScript(resDir, scriptName, logfile=None):
	disp('Running '+scriptName+' in '+resDir);
	if not os.path.exists(resDir):   disp('error directory'+resDir+' not present'); exit(-1)
	if not logfile:  logfile = open(resDir+'/'+scriptName.split(' ')[0]+'.log','wb')
	return subprocess.run('exec '+scriptName, stdout=logfile, stderr=logfile, shell=True, cwd=resDir) #, env=msEnv


def grepFloatInStr(lines='',keyword='Kx=', fileNameHint=''):
	try:
		for ele in re.split(':|=| |,|\n|\t|;',lines.split(keyword,1)[1],20) :
			if len(ele):
				try: return float(ele) ;   
				except ValueError: 
					DbgMsg('no valid data for '+keyword+' in '+fileNameHint+', got '+ele);  pass ; 
					return 0.0
	except :
		DbgMsg(keyword+' ?not in file '+fileNameHint); pass
	DbgMsg(keyword+' not in file '+fileNameHint)
	return 0.0

def grepFloatInFile(inFIle='summary.txt',keyword='Kx='):
	try:     lines = open(inFIle, 'r').read()
	except: DbgMsg('cannot open '+inFIle) ; return 0.0
	return grepFloatInStr(lines,keyword, inFIle)

def fileFloatDiffersFrom(inFIle, keyword, val, frac=0.01, delta=1.0e-32):
	fVal=grepFloatInFile(inFIle,keyword)
	if abs(fVal-val)<frac*abs(val)+delta : 
		print(inFIle+" -> "+keyword+": "+str(fVal) +" ~= "+str(val))
		return 0
	else                                  : 
		print(inFIle+" -> "+keyword+": "+str(fVal) +" != "+str(val))
		return 1
