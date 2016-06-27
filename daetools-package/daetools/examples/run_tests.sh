#!/bin/sh
# If called with no arguments the default python is used
# Otherwise, the argument represents the user-defined python

set -e

PYTHON=$1
echo $PYTHON

ROOT="$( cd "$( dirname "$0" )" && pwd )"

echo whats_the_time.py
$PYTHON $ROOT/whats_the_time.py console

echo tutorial1.py
$PYTHON $ROOT/tutorial1.py console

echo tutorial2.py
$PYTHON $ROOT/tutorial2.py console

echo tutorial3.py
$PYTHON $ROOT/tutorial3.py console

echo tutorial4.py
$PYTHON $ROOT/tutorial4.py console

echo tutorial5.py
$PYTHON $ROOT/tutorial5.py console

echo tutorial6.py
$PYTHON $ROOT/tutorial6.py console

echo tutorial7.py
$PYTHON $ROOT/tutorial7.py console

echo tutorial8.py
$PYTHON $ROOT/tutorial8.py console

echo tutorial9.py
$PYTHON $ROOT/tutorial9.py console

echo tutorial10.py
$PYTHON $ROOT/tutorial10.py console

echo tutorial11.py
$PYTHON $ROOT/tutorial11.py console

echo tutorial12.py
$PYTHON $ROOT/tutorial12.py console

echo tutorial13.py
$PYTHON $ROOT/tutorial13.py console

echo tutorial14.py
$PYTHON $ROOT/tutorial14.py console

echo tutorial15.py
$PYTHON $ROOT/tutorial15.py console

echo tutorial16.py
$PYTHON $ROOT/tutorial16.py console

echo tutorial17.py
$PYTHON $ROOT/tutorial17.py console

echo tutorial18.py
$PYTHON $ROOT/tutorial18.py console

echo tutorial19.py
$PYTHON $ROOT/tutorial19.py console

echo tutorial20.py
$PYTHON $ROOT/tutorial20.py console

echo tutorial21.py
$PYTHON $ROOT/tutorial21.py console

#echo tutorial22.py
#$PYTHON $ROOT/tutorial22.py console

echo tutorial23.py
$PYTHON $ROOT/tutorial23.py console

echo tutorial_dealii_1.py
$PYTHON $ROOT/tutorial_dealii_1.py console

echo tutorial_dealii_2.py
$PYTHON $ROOT/tutorial_dealii_2.py console

echo opt_tutorial1.py
$PYTHON $ROOT/opt_tutorial1.py console

echo opt_tutorial2.py
$PYTHON $ROOT/opt_tutorial2.py console

echo opt_tutorial3.py
$PYTHON $ROOT/opt_tutorial3.py console

echo opt_tutorial4.py
$PYTHON $ROOT/opt_tutorial4.py console

echo opt_tutorial5.py
$PYTHON $ROOT/opt_tutorial5.py console

echo opt_tutorial6.py
$PYTHON $ROOT/opt_tutorial6.py console

echo opt_tutorial7.py
$PYTHON $ROOT/opt_tutorial7.py console
