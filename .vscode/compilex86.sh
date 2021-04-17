type_dir="$1"

if [ -d "build" ]
then
    cd build
else
    mkdir build
    cd build
fi

if [ -d "$type_dir" ]
then
    cd $type_dir
else
    mkdir "$type_dir"
    cd "$type_dir"
fi

cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../..

make -r -s -j8