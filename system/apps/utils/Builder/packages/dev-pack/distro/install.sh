#! /bin/sh

packages='autoconf automake make binutils gcc patch m4 flex bison cvs indent'

if [ "$USER" != "root" ]
then
	echo "The installation is not being run by the administrative user."
	echo "Installation aborted."
	echo "You can log in as the \"root\" user and try again."
	exit 1
fi

echo "This will install the packages contained in the Developer Pack."
echo "Previously installed packages of the same name will be removed first."
echo ""
read -p "Do you want to continue (y/N)? " -e answer

if [ "$answer" != "y" ]
then
	echo "Installation aborted."
	exit 1
fi

echo ""

for package in $packages
do
	if [ -e "/usr/$package" ]
	then
		echo "Uninstalling existing /usr/$package"
		pkgmanager -r /usr/$package
		rm -r /usr/$package
		sync
	fi
done

echo ""

for package in `ls *.tgz`
do
	echo "Installing $package"
	tar -C /usr -xpPzf $package
	sync
done

for package in $packages
do
	echo ""
	echo "Registering $package"
	pkgmanager -a /usr/$package
	sync
done

echo ""
echo "Done"
