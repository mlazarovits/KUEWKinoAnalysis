#! /usr/bin/env python

import os, sys, commands, time

#look for the current directory
#######################################
pwd = os.environ['PWD']
home = os.environ['HOME']
#######################################
RUN_DIR = pwd
TEMP = pwd
EXE  = "MakeReducedNtuple.x"
#EXE  = "MakeEventCount.x"
OUT  = "/home/t3-ku/mlazarov/CMSSW_10_1_4_patch1/src/red_NTUPLES"
#OUT = pwd
LIST = "default.list"
QUEUE = ""
TREE = "stopTreeMaker/AUX"
MAXN = 500

def new_listfile(rootlist, listfile):
    mylist = open(listfile,'w')
    for f in rootlist:
        mylist.write(f+" \n")
    mylist.close()

def create_filelist(rootlist, filetag):
    listlist = []
    listcount = 0
    
    sublist = []
    for f in rootlist:
        sublist.append(f)
        if len(sublist) >= MAXN and MAXN > 0:
            listfile = "%s/%s_%d.list" % (listdir, filetag, listcount)
            new_listfile(sublist, listfile)
            listlist.append(listfile)
            sublist = []
            listcount += 1

    if len(sublist) > 0:
        listfile = "%s%s_%d.list" % (listdir, filetag, listcount)
        new_listfile(sublist, listfile)
        listlist.append(listfile)

    return listlist

def write_sh(srcfile,ifile,ofile,lfile,tag):
    fsrc = open(srcfile,'w')
    fsrc.write('universe = vanilla \n')
    fsrc.write('executable = '+EXE+" \n")
    fsrc.write('getenv = True \n')
    fsrc.write('Arguments = ');
    fsrc.write('-ilist='+ifile+" ")
    fsrc.write('-ofile='+ofile+" ")
    fsrc.write('-tree='+TREE+" ")
    if DO_SMS == 1:
        fsrc.write('--sms ')
    fsrc.write('-tag='+tag+" \n")
    fsrc.write('output = '+lfile+" \n")
    fsrc.write('queue \n')
    #fsrc.write('cd '+RUN_DIR+" \n")
    #fsrc.write('source ../RestFrames/setup_RestFrames.sh \n')
    fsrc.close()

if __name__ == "__main__":
    if not len(sys.argv) > 1 or '-h' in sys.argv or '--help' in sys.argv:
        print "Usage: %s [-q queue] [-tree treename] [-list listfile.list] [-maxN N] [--sms]" % sys.argv[0]
        print
        sys.exit(1)

    argv_pos = 1
    DO_SMS = 0
  
    if '-q' in sys.argv:
        p = sys.argv.index('-q')
        QUEUE = sys.argv[p+1]
        argv_pos += 2
    if '-list' in sys.argv:
        p = sys.argv.index('-list')
        LIST = sys.argv[p+1]
        argv_pos += 2
    if '-tree' in sys.argv:
        p = sys.argv.index('-tree')
        TREE = sys.argv[p+1]
        argv_pos += 2
    if '-maxN' in sys.argv:
        p = sys.argv.index('-maxN')
        MAXN = int(sys.argv[p+1])
        argv_pos += 2
    if '--sms' in sys.argv:
        DO_SMS = 1
        argv_pos += 1

    print "maxN is %d" % MAXN

    # input sample list
    listfile = LIST
    listname = listfile.split("/")
    listname = listname[-1]

    print listname

    NAME = listname.replace(".list",'')
    
    print NAME
    print RUN_DIR
        
    # create and organize output folders
    TARGET  = RUN_DIR+"/"+NAME+"/"
    os.system("rm -rf "+TARGET)
    os.system("mkdir -p "+TARGET)
    listdir = TARGET+"list/"
    srcdir  = TARGET+"src/"
    logdir  = TARGET+"log/"
    os.system("mkdir -p "+listdir)
    os.system("mkdir -p "+logdir)
    os.system("mkdir -p "+srcdir)
    
    # output root files
    ROOT = OUT+"/"+NAME+"/"
    if ROOT == TARGET:
        ROOT = ROOT+"root/"

    # make output folders
    os.system("rm -rf "+ROOT)
    os.system("mkdir -p "+ROOT)

    hints = []
    hints.append("0000")
    hints.append("0001")
    hints.append("0002")
    hints.append("0003")
    hints.append("0004")

    taglist = []
    
    with open(listfile,'r') as mylist:
        inputlist = mylist.readlines()

        # get dataset name (filetag) for bookkeeping
        for fold in inputlist:
            fold = fold.strip('\n\r')
            print "Processing folder %s" % fold
            filetag = fold.split("/")
            found = 0
            for hint in hints:
                index = 0
                for sub in filetag:
                    if hint in sub:
                        filetag = filetag[index-3]+NAME
                        found = 1
                        break
                    index += 1
                if found == 1:
                    break
            if found == 0:
                continue

            rootlist = [os.path.join(fold,f) for f in os.listdir(fold) if (os.path.isfile(os.path.join(fold, f)) and (".root" in f))]
            if len(rootlist) < 1:
                continue
            
            if len(taglist) == 0:
                taglist.append((filetag,rootlist))
                os.system("mkdir -p "+ROOT+filetag+"/")
                continue
            
            tagtuple = [item for item in taglist if item[0] == filetag]
            if len(tagtuple) == 0:
                taglist.append((filetag,rootlist))
                os.system("mkdir -p "+ROOT+filetag+"/")
                continue

            p = taglist.index(tagtuple[0])
            taglist[p][1].extend(rootlist)

    for (filetag,rootlist) in taglist:
        listlist = create_filelist(rootlist, filetag)

        for f in listlist:
            filename = f.split("/")
            filename = filename[-1]
            name = filename.replace(".list",'')
            write_sh(srcdir+name+".sh",f,ROOT+filetag+"/"+name+".root",logdir+name+".log",filetag)
            os.system('condor_submit '+srcdir+name+".sh")
            
    
