@ECHO OFF

REM Command file for Sphinx documentation

if "%1" == "html" (

    cd ko
    make html
    cd ..
    cd en
    make html
    cd ..
    goto end
)

if "%1" == "clean" (

    cd ko
    make clean
    cd ..
    cd en
    make clean
    cd ..
    goto end
)


:end
