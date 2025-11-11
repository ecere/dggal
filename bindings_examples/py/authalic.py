#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from dggal import *

app = Application(appGlobals=globals())
pydggal_setup(app)

cp = authalicSetup(wgs84Major, wgs84Minor)

lat = Degrees(45.0)

authalic = latGeodeticToAuthalic(cp, lat)

geodetic = latAuthalicToGeodetic(cp, authalic)

print("Authalic latitude:", Degrees(authalic))

print("Geodetic latitude:", Degrees(geodetic))
