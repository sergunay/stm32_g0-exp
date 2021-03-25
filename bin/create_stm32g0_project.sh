STM32G0_PROJ_PATH="/home/serg/ws/stm32/g0-exp/proj"
CMSIS_INC_PATH="/home/serg/ws/stm32/g0-exp/STM32CubeG0/Drivers/CMSIS/Core/Include"
ST_DEV_PATH="/home/serg/ws/stm32/g0-exp/STM32CubeG0/Drivers/CMSIS/Device/ST/STM32G0xx"
SE_TEMPLATES_PATH="/home/serg/ws/stm32/g0-exp/templates"

if [ -z "$1" ]; then
	echo "Project name is missing."
	exit
fi
 
mkdir -p $STM32G0_PROJ_PATH/$1
mkdir -p $STM32G0_PROJ_PATH/$1/bin
mkdir -p $STM32G0_PROJ_PATH/$1/inc
mkdir -p $STM32G0_PROJ_PATH/$1/src
mkdir -p $STM32G0_PROJ_PATH/$1/obj

cp $CMSIS_INC_PATH/cmsis_compiler.h $STM32G0_PROJ_PATH/$1/inc/
cp $CMSIS_INC_PATH/core_cm0plus.h $STM32G0_PROJ_PATH/$1/inc/
cp $CMSIS_INC_PATH/cmsis_gcc.h $STM32G0_PROJ_PATH/$1/inc/
cp $CMSIS_INC_PATH/cmsis_version.h $STM32G0_PROJ_PATH/$1/inc/
cp $CMSIS_INC_PATH/mpu_armv7.h $STM32G0_PROJ_PATH/$1/inc/

cp $ST_DEV_PATH/Include/stm32g0xx.h $STM32G0_PROJ_PATH/$1/inc/
cp $ST_DEV_PATH/Include/stm32g031xx.h $STM32G0_PROJ_PATH/$1/inc/
cp $ST_DEV_PATH/Include/system_stm32g0xx.h $STM32G0_PROJ_PATH/$1/inc/

cp $ST_DEV_PATH/Source/Templates/system_stm32g0xx.c $STM32G0_PROJ_PATH/$1/src/
cp $ST_DEV_PATH/Source/Templates/gcc/startup_stm32g031xx.s $STM32G0_PROJ_PATH/$1/src/

cp $SE_TEMPLATES_PATH/main.c $STM32G0_PROJ_PATH/$1/src/
cp $SE_TEMPLATES_PATH/Makefile $STM32G0_PROJ_PATH/$1/
cp $SE_TEMPLATES_PATH/.gitignore $STM32G0_PROJ_PATH/$1/
cp $SE_TEMPLATES_PATH/STM32G031K8Tx_FLASH.ld $STM32G0_PROJ_PATH/$1/
cp $SE_TEMPLATES_PATH/mylib.c $STM32G0_PROJ_PATH/$1/src/
cp $SE_TEMPLATES_PATH/mylib.h $STM32G0_PROJ_PATH/$1/inc/
cp $SE_TEMPLATES_PATH/isr.c $STM32G0_PROJ_PATH/$1/src/

cd $STM32G0_PROJ_PATH/$1/
