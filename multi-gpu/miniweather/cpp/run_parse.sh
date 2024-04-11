LOG_DIR="logs_all"
echo HIDING
python3 ./parse_miniweather_results.py ./${LOG_DIR}/miniweather_hiding.log

echo NO_HIDING
python3 ./parse_miniweather_results.py ./${LOG_DIR}/miniweather_no_hiding.log


echo KERNEL
python3 ./parse_miniweather_results.py ./${LOG_DIR}/miniweather_per_kernel.log

echo APP
python3 ./parse_miniweather_results.py ./${LOG_DIR}/miniweather_per_app.log