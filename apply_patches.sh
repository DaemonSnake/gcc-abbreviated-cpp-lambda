if [ $# -ne 1 ];
then
    echo "requires the path of the gcc folder";
    exit 42;
fi;

if [ ! -d patch/ ];
then
    echo "no patch/ folder found, make sure you are in the right folder";
    exit 42;
fi;

for patch in $(ls patch/*.patch);
do
    patch -d $1 -p1 < patch/$patch
done;
