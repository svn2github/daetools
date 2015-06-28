#!python
import os, sys, platform, shutil

if platform.system() == 'Windows':
    messages = []
    sys_drive = os.environ['SYSTEMDRIVE']
    config_dir = os.path.realpath( os.path.join(sys_drive, '\\daetools') )

    sm_dir        = get_special_folder_path('CSIDL_COMMON_STARTMENU')
    startmenu_dir = os.path.join(sm_dir, 'DAE Tools')
    desktop_dir   = get_special_folder_path("CSIDL_COMMON_DESKTOPDIRECTORY")

    pyw_executable = os.path.join(sys.prefix, "pythonw.exe")
    working_dir    = os.path.expanduser('~')
    
    if 'install' in sys.argv[1]:
        sm_dir      = get_special_folder_path("CSIDL_COMMON_STARTMENU")

        try:
            if not os.path.exists(config_dir):
                os.mkdir(config_dir)
        except Exception as e:
            messages.append(str(e))

        try:
            f = os.path.join(config_dir, 'daetools.cfg')
            if os.path.exists(f):
                os.remove(f)
            shutil.move(os.path.join(sys.prefix, 'daetools.cfg'), config_dir)
        except Exception as e:
            messages.append(str(e))

        try:
            f = os.path.join(config_dir, 'bonmin.cfg')
            if os.path.exists(f):
                os.remove(f)
            shutil.move(os.path.join(sys.prefix, 'bonmin.cfg'), config_dir)
        except Exception as e:
            messages.append(str(e))

        try:
            if not os.path.exists(startmenu_dir):
                os.mkdir(startmenu_dir)
        except Exception as e:
            messages.append(str(e))

        """
        try:
            f = os.path.join(startmenu_dir, 'daePlotter.lnk')
            if os.path.exists(f):
                os.remove(f)
            shutil.move(os.path.join(sys.prefix, 'daePlotter.lnk'), startmenu_dir)
        except Exception as e:
            messages.append(str(e))

        try:
            f = os.path.join(startmenu_dir, 'daeExamples.lnk')
            if os.path.exists(f):
                os.remove(f)
            shutil.move(os.path.join(sys.prefix, 'daeExamples.lnk'), startmenu_dir)
        except Exception as e:
            messages.append(str(e))
        """
        
        try:
            for path in [desktop_dir, startmenu_dir]:
                create_shortcut(pyw_executable,
                                'DAE Tools Examples',
                                os.path.join(path, 'daeExamples.lnk'),
                                '-c "from daetools.examples.run_examples import daeRunExamples; daeRunExamples()"',
                                working_dir)
                create_shortcut(pyw_executable,
                                'DAE Tools Plotter',
                                os.path.join(path, 'daePlotter.lnk'),
                                '-c "from daetools.dae_plotter.plotter import daeStartPlotter; daeStartPlotter()"',
                                working_dir)
        except Exception as e:
            messages.append(str(e))
                
        try:
            print '\n'.join(messages)
            sys.stdout.flush()
        except Exception as e:
            pass

    elif 'remove' in sys.argv[1]:
        try:
            shutil.removedirs(startmenu_dir)
            shutil.removedirs(config_dir)
        except Exception as e:
            messages.append(str(e))

    """
    try:
        f = open('c:\daetools-install.log', 'w')
        for line in sys.argv:
            f.write(line)
        for line in messages:
            f.write(line)
        f.close()
    except Exception as e:
        pass
    """
