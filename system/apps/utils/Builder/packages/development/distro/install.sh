#! /bin/sh

if [ `id --user` != 0 ]
then
	echo "The installation is not being run through the system (root) user."
	echo "Installation aborted."
	echo 'You can try again by using the "su" command.'
	exit 1
fi

echo "This will install the components contained in the developer files pack."
echo "Previously installed developer files will be deleted first."
echo ""
read -p "Do you want to continue (y/N)? " -e answer

if [ "$answer" != "y" ]
then
	echo "Installation aborted."
	exit 1
fi

echo ""

if [ -e "/system/development" ]
then
	for package in `ls /system/development/resources/`
	do
		for version in `ls /system/development/resources/$package/`
		do
			package unregister /system/development/resources/$package/$version /system/index
			sync
		done
	done

	echo "Deleting existing development files"
	rm -r /system/development
	sync
fi

echo ""
echo "Unpacking files"
unxz Syllable*-development*.xz
sync

echo ""
echo "Installing files"
unzip -q Syllable*-development*.zip -d /system
sync

for package in `ls /system/development/resources/`
do
	for version in `ls /system/development/resources/$package/`
	do
		echo ""
		package register /system/development/resources/$package/$version /system/index
		sync
	done
done

echo ""
echo "Done."
