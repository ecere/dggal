public import IMPORT_STATIC "ecrt"
private:

import "GeoExtent"

import "dggrs"
import "RI3H"

/*
   28 I3H Sub Zones Cases
   =========================                                               -- EXAMPLES --
         PARENT    SUB-ZONE  DEPTH     N/S    START   SCANLINES          | PARENT      P LEVEL  SZ LEVEL    DEPTH    SZ COUNT    FIRST
   Basic hexagons
    1.    Odd        Even    Odd       N&S    Edge    Left-Right           A6-0-E      1        4           3        37          C6-6-A
                                                                           [ "C6-6-A", "C6-10-A", "C6-1A-A", "C8-6-A", "C6-5-A", "C6-F-A", "C6-19-A", "C6-23-A", "C8-5-A", "C6-4-A", "C6-E-A", "C6-18-A", "C6-22-A", "C6-2C-A", "C8-4-A", "C6-3-A", "C6-D-A", "C6-17-A", "C6-21-A", "C6-2B-A", "C6-35-A", "C8-3-A", "C6-C-A", "C6-16-A", "C6-20-A", "C6-2A-A", "C6-34-A", "C6-3E-A", "C6-15-A", "C6-1F-A", "C6-29-A", "C6-33-A", "C6-3D-A", "C6-1E-A", "C6-28-A", "C6-32-A", "C6-3C-A" ]
    2.    Even       Odd     Odd       N&S    Edge    Bottom-Top           B6-5-A      2        5           3        37          C6-16-D
                                                                           [ "C6-16-D", "C6-D-F", "C6-D-E", "C6-E-D", "C6-16-F", "C6-16-E", "C6-17-D", "C6-E-F", "C6-E-E", "C6-1F-E", "C6-20-D", "C6-17-F", "C6-17-E", "C6-18-D", "C6-F-F", "C6-29-D", "C6-20-F", "C6-20-E", "C6-21-D", "C6-18-F", "C6-18-E", "C6-19-D", "C6-29-E", "C6-2A-D", "C6-21-F", "C6-21-E", "C6-22-D", "C6-19-F", "C6-2A-F", "C6-2A-E", "C6-2B-D", "C6-22-F", "C6-22-E", "C6-34-D", "C6-2B-F", "C6-2B-E", "C6-2C-D" ]
    3.    Even       Even    Even      N&S    Vertex  Left-Right           B6-5-A      2        4           2        13          C6-19-A
                                                                           [ "C6-19-A", "C6-E-A", "C6-18-A", "C6-22-A", "C6-2C-A", "C6-17-A", "C6-21-A", "C6-2B-A", "C6-16-A", "C6-20-A", "C6-2A-A", "C6-34-A", "C6-29-A" ]
                                                                           B6-5-A      2        6           4        91          D6-B7-A
                                                                           [ "D6-B7-A", "D6-9A-A", "D6-B6-A", "D6-D2-A", "D6-EE-A", "D6-7D-A", "D6-99-A", "D6-B5-A", "D6-D1-A", "D6-ED-A", "D6-109-A", "D6-125-A", "D6-60-A", "D6-7C-A", "D6-98-A", "D6-B4-A", "D6-D0-A", "D6-EC-A", "D6-108-A", "D6-124-A", "D6-140-A", "D6-15C-A", "D6-7B-A", "D6-97-A", "D6-B3-A", "D6-CF-A", "D6-EB-A", "D6-107-A", "D6-123-A", "D6-13F-A", "D6-15B-A", "D6-7A-A", "D6-96-A", "D6-B2-A", "D6-CE-A", "D6-EA-A", "D6-106-A", "D6-122-A", "D6-13E-A", "D6-15A-A", "D6-176-A", "D6-95-A", "D6-B1-A", "D6-CD-A", "D6-E9-A", "D6-105-A", "D6-121-A", "D6-13D-A", "D6-159-A", "D6-175-A", "D6-94-A", "D6-B0-A", "D6-CC-A", "D6-E8-A", "D6-104-A", "D6-120-A", "D6-13C-A", "D6-158-A", "D6-174-A", "D6-190-A", "D6-AF-A", "D6-CB-A", "D6-E7-A", "D6-103-A", "D6-11F-A", "D6-13B-A", "D6-157-A", "D6-173-A", "D6-18F-A", "D6-AE-A", "D6-CA-A", "D6-E6-A", "D6-102-A", "D6-11E-A", "D6-13A-A", "D6-156-A", "D6-172-A", "D6-18E-A", "D6-1AA-A", "D6-E5-A", "D6-101-A", "D6-11D-A", "D6-139-A", "D6-155-A", "D6-171-A", "D6-18D-A", "D6-11C-A", "D6-138-A", "D6-154-A", "D6-170-A", "D6-153-A" ]
    4.    Odd        Odd     Even      N&S    Vertex  Bottom-Top           A6-0-E      1        7           6        757         D6-9-D
                                                                           A6-0-E      1        5           4        91          C6-3-D
                                                                           [ "C6-3-D", "C6-C-D", "C6-3-F", "C6-3-E", "C6-4-D", "C6-15-D", "C6-C-F", "C6-C-E", "C6-D-D", "C6-4-F", "C6-4-E", "C6-5-D", "C6-1E-D", "C6-15-F", "C6-15-E", "C6-16-D", "C6-D-F", "C6-D-E", "C6-E-D", "C6-5-F", "C6-5-E", "C6-6-D", "C6-1E-E", "C6-1F-D", "C6-16-F", "C6-16-E", "C6-17-D", "C6-E-F", "C6-E-E", "C6-F-D", "C6-6-F", "C6-28-D", "C6-1F-F", "C6-1F-E", "C6-20-D", "C6-17-F", "C6-17-E", "C6-18-D", "C6-F-F", "C6-F-E", "C6-10-D", "C6-28-E", "C6-29-D", "C6-20-F", "C6-20-E", "C6-21-D", "C6-18-F", "C6-18-E", "C6-19-D", "C6-10-F", "C6-32-D", "C6-29-F", "C6-29-E", "C6-2A-D", "C6-21-F", "C6-21-E", "C6-22-D", "C6-19-F", "C6-19-E", "C6-1A-D", "C6-32-E", "C6-33-D", "C6-2A-F", "C6-2A-E", "C6-2B-D", "C6-22-F", "C6-22-E", "C6-23-D", "C6-1A-F", "C6-3C-D", "C6-33-F", "C6-33-E", "C6-34-D", "C6-2B-F", "C6-2B-E", "C6-2C-D", "C6-23-F", "C6-23-E", "C8-6-D", "C6-3D-D", "C6-34-F", "C6-34-E", "C6-35-D", "C6-2C-F", "C6-2C-E", "C8-5-D", "C6-3E-D", "C6-35-F", "C6-35-E", "C8-4-D", "C8-3-D" ]

   Interruption-spanning hexagons
    5.    Odd        Even    Odd       North  Edge    Left-Right           B6-1-D      3        6           3        37          D4-1AD-A
                                                                           [ "D4-1AD-A", "D4-1AE-A", "D4-1AF-A", "D6-C-A", "D4-1C8-A", "D4-1C9-A", "D4-1CA-A", "D6-B-A", "D6-27-A", "D4-1E3-A", "D4-1E4-A", "D4-1E5-A", "D6-A-A", "D6-26-A", "D6-42-A", "D4-1FE-A", "D4-1FF-A", "D4-200-A", "D6-9-A", "D6-25-A", "D6-41-A", "D6-5D-A", "D4-21A-A", "D4-21B-A", "D6-8-A", "D6-24-A", "D6-40-A", "D6-5C-A", "D4-236-A", "D6-7-A", "D6-23-A", "D6-3F-A", "D6-5B-A", "D6-6-A", "D6-22-A", "D6-3E-A", "D6-5A-A" ]
    6.    Odd        Even    Odd       South  Edge    Left-Right           B5-3-D      3        6           3        37          D3-29A-A
                                                                           [ "D3-29A-A", "D3-2B6-A", "D3-2D2-A", "D5-A2-A", "D3-299-A", "D3-2B5-A", "D3-2D1-A", "D5-BD-A", "D5-BE-A", "D3-298-A", "D3-2B4-A", "D3-2D0-A", "D5-D8-A", "D5-D9-A", "D5-DA-A", "D3-297-A", "D3-2B3-A", "D3-2CF-A", "D5-F3-A", "D5-F4-A", "D5-F5-A", "D5-F6-A", "D3-2B2-A", "D3-2CE-A", "D5-10E-A", "D5-10F-A", "D5-110-A", "D5-111-A", "D3-2CD-A", "D5-129-A", "D5-12A-A", "D5-12B-A", "D5-12C-A", "D5-144-A", "D5-145-A", "D5-146-A", "D5-147-A" ]
    7.    Even       Odd     Odd       North  Edge    Bottom-Top           B6-1-A      2        7           5        271         D4-1AA-D
                                                                           [ "D4-1AA-D", "D4-18F-F", "D4-18F-E", "D4-190-D", "D4-175-F", "D4-175-E", "D4-176-D", "D4-15B-F", "D4-15B-E", "D4-15C-D", "D4-1AA-F", "D4-1AA-E", "D4-1AB-D", "D4-190-F", "D4-190-E", "D4-191-D", "D4-176-F", "D4-176-E", "D4-177-D", "D4-15C-F", "D4-15C-E", "D4-1C5-E", "D4-1C6-D", "D4-1AB-F", "D4-1AB-E", "D4-1AC-D", "D4-191-F", "D4-191-E", "D4-192-D", "D4-177-F", "D4-177-E", "D4-178-D", "D4-15D-F", "D4-1E1-D", "D4-1C6-F", "D4-1C6-E", "D4-1C7-D", "D4-1AC-F", "D4-1AC-E", "D4-1AD-D", "D4-192-F", "D4-192-E", "D4-193-D", "D4-178-F", "D4-178-E", "D4-179-D", "D4-1E1-F", "D4-1E1-E", "D4-1E2-D", "D4-1C7-F", "D4-1C7-E", "D4-1C8-D", "D4-1AD-F", "D4-1AD-E", "D4-1AE-D", "D4-193-F", "D4-193-E", "D4-194-D", "D4-179-F", "D4-179-E", "D4-1FC-E", "D4-1FD-D", "D4-1E2-F", "D4-1E2-E", "D4-1E3-D", "D4-1C8-F", "D4-1C8-E", "D4-1C9-D", "D4-1AE-F", "D4-1AE-E", "D4-1AF-D", "D4-194-F", "D4-194-E", "D6-D-D", "D6-D-E", "D4-218-D", "D4-1FD-F", "D4-1FD-E", "D4-1FE-D", "D4-1E3-F", "D4-1E3-E", "D4-1E4-D", "D4-1C9-F", "D4-1C9-E", "D4-1CA-D", "D4-1AF-F", "D4-1AF-E", "D6-C-D", "D6-C-E", "D6-D-F", "D6-29-D", "D4-218-F", "D4-218-E", "D4-219-D", "D4-1FE-F", "D4-1FE-E", "D4-1FF-D", "D4-1E4-F", "D4-1E4-E", "D4-1E5-D", "D4-1CA-F", "D4-1CA-E", "D6-B-D", "D6-B-E", "D6-C-F", "D6-28-D", "D6-28-E", "D6-29-F", "D4-233-E", "D4-234-D", "D4-219-F", "D4-219-E", "D4-21A-D", "D4-1FF-F", "D4-1FF-E", "D4-200-D", "D4-1E5-F", "D4-1E5-E", "D6-A-D", "D6-A-E", "D6-B-F", "D6-27-D", "D6-27-E", "D6-28-F", "D6-44-D", "D6-44-E", "D4-24F-D", "D4-234-F", "D4-234-E", "D4-235-D", "D4-21A-F", "D4-21A-E", "D4-21B-D", "D4-200-F", "D4-200-E", "D6-9-D", "D6-9-E", "D6-A-F", "D6-26-D", "D6-26-E", "D6-27-F", "D6-43-D", "D6-43-E", "D6-44-F", "D6-60-D", "D4-24F-E", "D4-250-D", "D4-235-F", "D4-235-E", "D4-236-D", "D4-21B-F", "D4-21B-E", "D6-8-D", "D6-8-E", "D6-9-F", "D6-25-D", "D6-25-E", "D6-26-F", "D6-42-D", "D6-42-E", "D6-43-F", "D6-5F-D", "D6-5F-E", "D4-250-F", "D4-250-E", "D4-251-D", "D4-236-F", "D4-236-E", "D6-7-D", "D6-7-E", "D6-8-F", "D6-24-D", "D6-24-E", "D6-25-F", "D6-41-D", "D6-41-E", "D6-42-F", "D6-5E-D", "D6-5E-E", "D6-5F-F", "D4-26C-D", "D4-251-F", "D4-251-E", "D6-6-D", "D6-6-E", "D6-7-F", "D6-23-D", "D6-23-E", "D6-24-F", "D6-40-D", "D6-40-E", "D6-41-F", "D6-5D-D", "D6-5D-E", "D6-5E-F", "D6-7A-D", "D4-26C-E", "D6-5-D", "D6-5-E", "D6-6-F", "D6-22-D", "D6-22-E", "D6-23-F", "D6-3F-D", "D6-3F-E", "D6-40-F", "D6-5C-D", "D6-5C-E", "D6-5D-F", "D6-79-D", "D6-79-E", "D6-4-E", "D6-5-F", "D6-21-D", "D6-21-E", "D6-22-F", "D6-3E-D", "D6-3E-E", "D6-3F-F", "D6-5B-D", "D6-5B-E", "D6-5C-F", "D6-78-D", "D6-78-E", "D6-79-F", "D6-20-D", "D6-20-E", "D6-21-F", "D6-3D-D", "D6-3D-E", "D6-3E-F", "D6-5A-D", "D6-5A-E", "D6-5B-F", "D6-77-D", "D6-77-E", "D6-78-F", "D6-94-D", "D6-20-F", "D6-3C-D", "D6-3C-E", "D6-3D-F", "D6-59-D", "D6-59-E", "D6-5A-F", "D6-76-D", "D6-76-E", "D6-77-F", "D6-93-D", "D6-93-E", "D6-3B-E", "D6-3C-F", "D6-58-D", "D6-58-E", "D6-59-F", "D6-75-D", "D6-75-E", "D6-76-F", "D6-92-D", "D6-92-E", "D6-93-F", "D6-57-D", "D6-57-E", "D6-58-F", "D6-74-D", "D6-74-E", "D6-75-F", "D6-91-D", "D6-91-E", "D6-92-F", "D6-AE-D" ]
                                                                           B6-1-A      2        5           3        37          C4-34-D
                                                                           [ "C4-34-D", "C4-2B-F", "C4-2B-E", "C4-2C-D", "C4-34-F", "C4-34-E", "C4-35-D", "C4-2C-F", "C4-2C-E", "C4-3D-E", "C4-3E-D", "C4-35-F", "C4-35-E", "C6-4-D", "C6-4-E", "C4-47-D", "C4-3E-F", "C4-3E-E", "C6-3-D", "C6-3-E", "C6-4-F", "C6-E-D", "C4-47-E", "C6-2-D", "C6-2-E", "C6-3-F", "C6-D-D", "C6-D-E", "C6-1-E", "C6-2-F", "C6-C-D", "C6-C-E", "C6-D-F", "C6-B-D", "C6-B-E", "C6-C-F", "C6-16-D" ]
    8.    Even       Odd     Odd       South  Edge    Bottom-Top           B5-3-A      2        7           5        271         D3-294-D
                                                                           [ "D3-294-D", "D3-279-F", "D3-279-E", "D3-27A-D", "D3-25F-F", "D3-25F-E", "D3-260-D", "D3-245-F", "D3-245-E", "D3-246-D", "D3-294-F", "D3-294-E", "D3-295-D", "D3-27A-F", "D3-27A-E", "D3-27B-D", "D3-260-F", "D3-260-E", "D3-261-D", "D3-246-F", "D3-246-E", "D3-2AF-E", "D3-2B0-D", "D3-295-F", "D3-295-E", "D3-296-D", "D3-27B-F", "D3-27B-E", "D3-27C-D", "D3-261-F", "D3-261-E", "D3-262-D", "D3-247-F", "D3-2CB-D", "D3-2B0-F", "D3-2B0-E", "D3-2B1-D", "D3-296-F", "D3-296-E", "D3-297-D", "D3-27C-F", "D3-27C-E", "D3-27D-D", "D3-262-F", "D3-262-E", "D3-263-D", "D3-2CB-F", "D3-2CB-E", "D3-2CC-D", "D3-2B1-F", "D3-2B1-E", "D3-2B2-D", "D3-297-F", "D3-297-E", "D3-298-D", "D3-27D-F", "D3-27D-E", "D3-27E-D", "D3-263-F", "D3-263-E", "D5-15F-F", "D5-15F-D", "D3-2CC-F", "D3-2CC-E", "D3-2CD-D", "D3-2B2-F", "D3-2B2-E", "D3-2B3-D", "D3-298-F", "D3-298-E", "D3-299-D", "D3-27E-F", "D3-27E-E", "D3-27F-D", "D3-264-F", "D5-17B-D", "D5-15F-E", "D5-144-F", "D5-144-D", "D3-2CD-F", "D3-2CD-E", "D3-2CE-D", "D3-2B3-F", "D3-2B3-E", "D3-2B4-D", "D3-299-F", "D3-299-E", "D3-29A-D", "D3-27F-F", "D3-27F-E", "D3-280-D", "D5-17B-E", "D5-160-F", "D5-160-D", "D5-144-E", "D5-129-F", "D5-129-D", "D3-2CE-F", "D3-2CE-E", "D3-2CF-D", "D3-2B4-F", "D3-2B4-E", "D3-2B5-D", "D3-29A-F", "D3-29A-E", "D3-29B-D", "D3-280-F", "D3-280-E", "D5-17C-F", "D5-17C-D", "D5-160-E", "D5-145-F", "D5-145-D", "D5-129-E", "D5-10E-F", "D5-10E-D", "D3-2CF-F", "D3-2CF-E", "D3-2D0-D", "D3-2B5-F", "D3-2B5-E", "D3-2B6-D", "D3-29B-F", "D3-29B-E", "D3-29C-D", "D3-281-F", "D5-198-D", "D5-17C-E", "D5-161-F", "D5-161-D", "D5-145-E", "D5-12A-F", "D5-12A-D", "D5-10E-E", "D5-F3-F", "D5-F3-D", "D3-2D0-F", "D3-2D0-E", "D3-2D1-D", "D3-2B6-F", "D3-2B6-E", "D3-2B7-D", "D3-29C-F", "D3-29C-E", "D3-29D-D", "D5-17D-F", "D5-17D-D", "D5-161-E", "D5-146-F", "D5-146-D", "D5-12A-E", "D5-10F-F", "D5-10F-D", "D5-F3-E", "D5-D8-F", "D5-D8-D", "D3-2D1-F", "D3-2D1-E", "D3-2D2-D", "D3-2B7-F", "D3-2B7-E", "D3-2B8-D", "D3-29D-F", "D5-17D-E", "D5-162-F", "D5-162-D", "D5-146-E", "D5-12B-F", "D5-12B-D", "D5-10F-E", "D5-F4-F", "D5-F4-D", "D5-D8-E", "D5-BD-F", "D5-BD-D", "D3-2D2-F", "D3-2D2-E", "D3-2D3-D", "D3-2B8-F", "D3-2B8-E", "D5-17E-D", "D5-162-E", "D5-147-F", "D5-147-D", "D5-12B-E", "D5-110-F", "D5-110-D", "D5-F4-E", "D5-D9-F", "D5-D9-D", "D5-BD-E", "D5-A2-F", "D5-A2-D", "D3-2D3-F", "D3-2D3-E", "D3-2D4-D", "D5-163-F", "D5-163-D", "D5-147-E", "D5-12C-F", "D5-12C-D", "D5-110-E", "D5-F5-F", "D5-F5-D", "D5-D9-E", "D5-BE-F", "D5-BE-D", "D5-A2-E", "D5-87-F", "D5-87-D", "D3-2D4-F", "D5-163-E", "D5-148-F", "D5-148-D", "D5-12C-E", "D5-111-F", "D5-111-D", "D5-F5-E", "D5-DA-F", "D5-DA-D", "D5-BE-E", "D5-A3-F", "D5-A3-D", "D5-87-E", "D5-6C-F", "D5-164-D", "D5-148-E", "D5-12D-F", "D5-12D-D", "D5-111-E", "D5-F6-F", "D5-F6-D", "D5-DA-E", "D5-BF-F", "D5-BF-D", "D5-A3-E", "D5-88-F", "D5-88-D", "D5-149-F", "D5-149-D", "D5-12D-E", "D5-112-F", "D5-112-D", "D5-F6-E", "D5-DB-F", "D5-DB-D", "D5-BF-E", "D5-A4-F", "D5-A4-D", "D5-88-E", "D5-149-E", "D5-12E-F", "D5-12E-D", "D5-112-E", "D5-F7-F", "D5-F7-D", "D5-DB-E", "D5-C0-F", "D5-C0-D", "D5-A4-E", "D5-89-F", "D5-14A-D", "D5-12E-E", "D5-113-F", "D5-113-D", "D5-F7-E", "D5-DC-F", "D5-DC-D", "D5-C0-E", "D5-A5-F", "D5-A5-D" ]
                                                                           B5-3-A      2        5           3        37          C3-4C-D
                                                                           [ "C3-4C-D", "C3-43-F", "C3-43-E", "C3-44-D", "C3-4C-F", "C3-4C-E", "C3-4D-D", "C3-44-F", "C3-44-E", "C5-24-F", "C5-24-D", "C3-4D-F", "C3-4D-E", "C3-4E-D", "C3-45-F", "C5-2E-D", "C5-24-E", "C5-1B-F", "C5-1B-D", "C3-4E-F", "C3-4E-E", "C3-4F-D", "C5-25-F", "C5-25-D", "C5-1B-E", "C5-12-F", "C5-12-D", "C3-4F-F", "C5-25-E", "C5-1C-F", "C5-1C-D", "C5-12-E", "C5-9-F", "C5-26-D", "C5-1C-E", "C5-13-F", "C5-13-D" ]
    9.    Even       Even    Even      North  Vertex  Left-Right           B6-1-A      2        6           4        91          D4-15C-A
                                                                           [ "D4-15C-A", "D4-176-A", "D4-177-A", "D4-178-A", "D4-179-A", "D4-190-A", "D4-191-A", "D4-192-A", "D4-193-A", "D4-194-A", "D6-D-A", "D6-29-A", "D4-1AA-A", "D4-1AB-A", "D4-1AC-A", "D4-1AD-A", "D4-1AE-A", "D4-1AF-A", "D6-C-A", "D6-28-A", "D6-44-A", "D6-60-A", "D4-1C6-A", "D4-1C7-A", "D4-1C8-A", "D4-1C9-A", "D4-1CA-A", "D6-B-A", "D6-27-A", "D6-43-A", "D6-5F-A", "D4-1E1-A", "D4-1E2-A", "D4-1E3-A", "D4-1E4-A", "D4-1E5-A", "D6-A-A", "D6-26-A", "D6-42-A", "D6-5E-A", "D6-7A-A", "D4-1FD-A", "D4-1FE-A", "D4-1FF-A", "D4-200-A", "D6-9-A", "D6-25-A", "D6-41-A", "D6-5D-A", "D6-79-A", "D4-218-A", "D4-219-A", "D4-21A-A", "D4-21B-A", "D6-8-A", "D6-24-A", "D6-40-A", "D6-5C-A", "D6-78-A", "D6-94-A", "D4-234-A", "D4-235-A", "D4-236-A", "D6-7-A", "D6-23-A", "D6-3F-A", "D6-5B-A", "D6-77-A", "D6-93-A", "D4-24F-A", "D4-250-A", "D4-251-A", "D6-6-A", "D6-22-A", "D6-3E-A", "D6-5A-A", "D6-76-A", "D6-92-A", "D6-AE-A", "D4-26C-A", "D6-5-A", "D6-21-A", "D6-3D-A", "D6-59-A", "D6-75-A", "D6-91-A", "D6-20-A", "D6-3C-A", "D6-58-A", "D6-74-A", "D6-57-A" ]
   10.    Even       Even    Even      South  Vertex  Left-Right           B5-3-A      2        6           4        91          D3-29D-A
                                                                           [ "D3-29D-A", "D3-280-A", "D3-29C-A", "D3-2B8-A", "D3-2D4-A", "D3-263-A", "D3-27F-A", "D3-29B-A", "D3-2B7-A", "D3-2D3-A", "D5-87-A", "D5-88-A", "D3-246-A", "D3-262-A", "D3-27E-A", "D3-29A-A", "D3-2B6-A", "D3-2D2-A", "D5-A2-A", "D5-A3-A", "D5-A4-A", "D5-A5-A", "D3-261-A", "D3-27D-A", "D3-299-A", "D3-2B5-A", "D3-2D1-A", "D5-BD-A", "D5-BE-A", "D5-BF-A", "D5-C0-A", "D3-260-A", "D3-27C-A", "D3-298-A", "D3-2B4-A", "D3-2D0-A", "D5-D8-A", "D5-D9-A", "D5-DA-A", "D5-DB-A", "D5-DC-A", "D3-27B-A", "D3-297-A", "D3-2B3-A", "D3-2CF-A", "D5-F3-A", "D5-F4-A", "D5-F5-A", "D5-F6-A", "D5-F7-A", "D3-27A-A", "D3-296-A", "D3-2B2-A", "D3-2CE-A", "D5-10E-A", "D5-10F-A", "D5-110-A", "D5-111-A", "D5-112-A", "D5-113-A", "D3-295-A", "D3-2B1-A", "D3-2CD-A", "D5-129-A", "D5-12A-A", "D5-12B-A", "D5-12C-A", "D5-12D-A", "D5-12E-A", "D3-294-A", "D3-2B0-A", "D3-2CC-A", "D5-144-A", "D5-145-A", "D5-146-A", "D5-147-A", "D5-148-A", "D5-149-A", "D5-14A-A", "D3-2CB-A", "D5-15F-A", "D5-160-A", "D5-161-A", "D5-162-A", "D5-163-A", "D5-164-A", "D5-17B-A", "D5-17C-A", "D5-17D-A", "D5-17E-A", "D5-198-A" ]
   11.    Odd        Odd     Even      North  Vertex  Bottom-Top           B6-1-D      3        7           4        91          D4-1AD-D
                                                                           [ "D4-1AD-D", "D4-1C8-D", "D4-1AD-F", "D4-1AD-E", "D4-1AE-D", "D4-1E3-D", "D4-1C8-F", "D4-1C8-E", "D4-1C9-D", "D4-1AE-F", "D4-1AE-E", "D4-1AF-D", "D4-1FE-D", "D4-1E3-F", "D4-1E3-E", "D4-1E4-D", "D4-1C9-F", "D4-1C9-E", "D4-1CA-D", "D4-1AF-F", "D4-1AF-E", "D6-C-D", "D4-1FE-E", "D4-1FF-D", "D4-1E4-F", "D4-1E4-E", "D4-1E5-D", "D4-1CA-F", "D4-1CA-E", "D6-B-D", "D6-B-E", "D4-21A-D", "D4-1FF-F", "D4-1FF-E", "D4-200-D", "D4-1E5-F", "D4-1E5-E", "D6-A-D", "D6-A-E", "D6-B-F", "D6-27-D", "D4-21A-E", "D4-21B-D", "D4-200-F", "D4-200-E", "D6-9-D", "D6-9-E", "D6-A-F", "D6-26-D", "D6-26-E", "D4-236-D", "D4-21B-F", "D4-21B-E", "D6-8-D", "D6-8-E", "D6-9-F", "D6-25-D", "D6-25-E", "D6-26-F", "D6-42-D", "D4-236-E", "D6-7-D", "D6-7-E", "D6-8-F", "D6-24-D", "D6-24-E", "D6-25-F", "D6-41-D", "D6-41-E", "D6-6-D", "D6-6-E", "D6-7-F", "D6-23-D", "D6-23-E", "D6-24-F", "D6-40-D", "D6-40-E", "D6-41-F", "D6-5D-D", "D6-22-D", "D6-22-E", "D6-23-F", "D6-3F-D", "D6-3F-E", "D6-40-F", "D6-5C-D", "D6-3E-D", "D6-3E-E", "D6-3F-F", "D6-5B-D", "D6-5A-D" ]
   12.    Odd        Odd     Even      South  Vertex  Bottom-Top           B5-3-D      3        7           4        91          D3-297-D
                                                                           [ "D3-297-D", "D3-2B2-D", "D3-297-F", "D3-297-E", "D3-298-D", "D3-2CD-D", "D3-2B2-F", "D3-2B2-E", "D3-2B3-D", "D3-298-F", "D3-298-E", "D3-299-D", "D5-144-D", "D3-2CD-F", "D3-2CD-E", "D3-2CE-D", "D3-2B3-F", "D3-2B3-E", "D3-2B4-D", "D3-299-F", "D3-299-E", "D3-29A-D", "D5-129-F", "D5-129-D", "D3-2CE-F", "D3-2CE-E", "D3-2CF-D", "D3-2B4-F", "D3-2B4-E", "D3-2B5-D", "D3-29A-F", "D5-145-D", "D5-129-E", "D5-10E-F", "D5-10E-D", "D3-2CF-F", "D3-2CF-E", "D3-2D0-D", "D3-2B5-F", "D3-2B5-E", "D3-2B6-D", "D5-12A-F", "D5-12A-D", "D5-10E-E", "D5-F3-F", "D5-F3-D", "D3-2D0-F", "D3-2D0-E", "D3-2D1-D", "D3-2B6-F", "D5-146-D", "D5-12A-E", "D5-10F-F", "D5-10F-D", "D5-F3-E", "D5-D8-F", "D5-D8-D", "D3-2D1-F", "D3-2D1-E", "D3-2D2-D", "D5-12B-F", "D5-12B-D", "D5-10F-E", "D5-F4-F", "D5-F4-D", "D5-D8-E", "D5-BD-F", "D5-BD-D", "D3-2D2-F", "D5-147-D", "D5-12B-E", "D5-110-F", "D5-110-D", "D5-F4-E", "D5-D9-F", "D5-D9-D", "D5-BD-E", "D5-A2-F", "D5-A2-D", "D5-12C-D", "D5-110-E", "D5-F5-F", "D5-F5-D", "D5-D9-E", "D5-BE-F", "D5-BE-D", "D5-111-D", "D5-F5-E", "D5-DA-F", "D5-DA-D", "D5-F6-D" ]

   Non-polar pentagons
   13.    Odd        Even    Odd       North  Edge    Left-Right           A6-0-D      1        4           3        31          C4-4C-A
                                                                           [ "C4-3C-A", "C4-3D-A", "C4-3E-A", "C6-3-A", "C4-45-A", "C4-46-A", "C4-47-A", "C6-2-A", "C6-C-A", "C4-4E-A", "C4-4F-A", "C4-50-A", "C6-1-A", "C6-B-A", "C6-15-A", "C5-6-A", "C5-7-A", "C5-8-A", "C6-0-A", "C6-A-A", "C6-14-A", "C6-1E-A", "C5-10-A", "C5-11-A", "C6-9-A", "C6-13-A", "C6-1D-A", "C5-1A-A", "C6-12-A", "C6-1C-A", "C6-1B-A" ]
   14.    Odd        Even    Odd       South  Edge    Left-Right           A5-0-D      1        4           3        31          C4-36-A
                                                                           [ "C4-36-A", "C4-40-A", "C4-4A-A", "C5-3-A", "C3-3E-A", "C4-3F-A", "C4-49-A", "C5-2-A", "C5-C-A", "C3-3D-A", "C3-47-A", "C4-48-A", "C5-1-A", "C5-B-A", "C5-15-A", "C3-3C-A", "C3-46-A", "C3-50-A", "C5-0-A", "C5-A-A", "C5-14-A", "C5-1E-A", "C3-45-A", "C3-4F-A", "C5-9-A", "C5-13-A", "C5-1D-A", "C3-4E-A", "C5-12-A", "C5-1C-A", "C5-1B-A" ]
   15.    Even       Odd     Odd       North  Edge    Bottom-Top           A2-0-A      0        3           3        31          B0-7-D
                                                                           [ "B0-7-D", "B0-4-F", "B0-4-E", "B0-5-D", "B0-7-F", "B0-7-E", "B0-8-D", "B0-5-F", "B0-5-E", "B1-1-E", "B1-2-D", "B0-8-F", "B0-8-E", "B2-1-D", "B2-1-E", "B1-5-D", "B1-2-F", "B1-2-E", "B2-0-D", "B2-0-E", "B2-1-F", "B2-5-D", "B1-5-E", "B2-3-D", "B2-0-F", "B2-4-D", "B2-4-E", "B2-3-F", "B2-3-E", "B2-4-F", "B2-7-D" ]
   16.    Even       Odd     Odd       South  Edge    Bottom-Top           A3-0-A      0        3           3        31          B1-7-D
                                                                           [ "B1-7-D", "B1-4-F", "B1-4-E", "B1-5-D", "B1-7-F", "B1-7-E", "B1-8-D", "B1-5-F", "B1-5-E", "B3-3-F", "B3-3-D", "B1-8-F", "B1-8-E", "B2-6-D", "B2-3-F", "B3-7-D", "B3-3-E", "B3-0-F", "B3-0-D", "B2-6-F", "B2-6-E", "B2-7-D", "B3-4-F", "B3-4-D", "B3-0-E", "B3-1-D", "B2-7-F", "B3-4-E", "B3-1-F", "B3-1-E", "B3-5-D" ]
   17.    Even       Even    Even      North  Vertex  Left-Right           A2-0-A      0        6           6        631         D0-105-A
                                                                           B2-0-A      2        6           4        76          D0-24F-A
                                                                           [ "D0-24F-A", "D0-269-A", "D0-26A-A", "D0-26B-A", "D0-26C-A", "D0-283-A", "D0-284-A", "D0-285-A", "D0-286-A", "D0-287-A", "D2-4-A", "D2-20-A", "D0-29D-A", "D0-29E-A", "D0-29F-A", "D0-2A0-A", "D0-2A1-A", "D0-2A2-A", "D2-3-A", "D2-1F-A", "D2-3B-A", "D2-57-A", "D0-2B9-A", "D0-2BA-A", "D0-2BB-A", "D0-2BC-A", "D0-2BD-A", "D2-2-A", "D2-1E-A", "D2-3A-A", "D2-56-A", "D0-2D4-A", "D0-2D5-A", "D0-2D6-A", "D0-2D7-A", "D0-2D8-A", "D2-1-A", "D2-1D-A", "D2-39-A", "D2-55-A", "D2-71-A", "D1-17-A", "D1-18-A", "D1-19-A", "D1-1A-A", "D2-0-A", "D2-1C-A", "D2-38-A", "D2-54-A", "D2-70-A", "D1-32-A", "D1-33-A", "D1-34-A", "D1-35-A", "D2-1B-A", "D2-37-A", "D2-53-A", "D2-6F-A", "D2-8B-A", "D1-4E-A", "D1-4F-A", "D1-50-A", "D2-36-A", "D2-52-A", "D2-6E-A", "D2-8A-A", "D1-69-A", "D1-6A-A", "D1-6B-A", "D2-51-A", "D2-6D-A", "D2-89-A", "D2-A5-A", "D1-86-A", "D2-6C-A", "D2-88-A" ]
                                                                           A2-0-A      0        4           4        76          C0-2C-A
                                                                           [ "C0-21-A", "C0-29-A", "C0-2A-A", "C0-2B-A", "C0-2C-A", "C0-31-A", "C0-32-A", "C0-33-A", "C0-34-A", "C0-35-A", "C2-4-A", "C2-E-A", "C0-39-A", "C0-3A-A", "C0-3B-A", "C0-3C-A", "C0-3D-A", "C0-3E-A", "C2-3-A", "C2-D-A", "C2-17-A", "C2-21-A", "C0-43-A", "C0-44-A", "C0-45-A", "C0-46-A", "C0-47-A", "C2-2-A", "C2-C-A", "C2-16-A", "C2-20-A", "C0-4C-A", "C0-4D-A", "C0-4E-A", "C0-4F-A", "C0-50-A", "C2-1-A", "C2-B-A", "C2-15-A", "C2-1F-A", "C2-29-A", "C1-5-A", "C1-6-A", "C1-7-A", "C1-8-A", "C2-0-A", "C2-A-A", "C2-14-A", "C2-1E-A", "C2-28-A", "C1-E-A", "C1-F-A", "C1-10-A", "C1-11-A", "C2-9-A", "C2-13-A", "C2-1D-A", "C2-27-A", "C2-31-A", "C1-18-A", "C1-19-A", "C1-1A-A", "C2-12-A", "C2-1C-A", "C2-26-A", "C2-30-A", "C1-21-A", "C1-22-A", "C1-23-A", "C2-1B-A", "C2-25-A", "C2-2F-A", "C2-39-A", "C1-2C-A", "C2-24-A", "C2-2E-A" ]
   18.    Even       Even    Even      South  Vertex  Left-Right           A3-0-A      0        4           4        76          C2-39-A
                                                                           [ "C2-39-A", "C2-2E-A", "C2-38-A", "C2-42-A", "C2-4C-A", "C1-2C-A", "C2-2D-A", "C2-37-A", "C2-41-A", "C2-4B-A", "C3-4-A", "C3-E-A", "C1-21-A", "C1-2B-A", "C1-35-A", "C2-36-A", "C2-40-A", "C2-4A-A", "C3-3-A", "C3-D-A", "C3-17-A", "C3-21-A", "C1-2A-A", "C1-34-A", "C1-3E-A", "C2-3F-A", "C2-49-A", "C3-2-A", "C3-C-A", "C3-16-A", "C3-20-A", "C1-29-A", "C1-33-A", "C1-3D-A", "C1-47-A", "C2-48-A", "C3-1-A", "C3-B-A", "C3-15-A", "C3-1F-A", "C3-29-A", "C1-32-A", "C1-3C-A", "C1-46-A", "C1-50-A", "C3-0-A", "C3-A-A", "C3-14-A", "C3-1E-A", "C3-28-A", "C1-31-A", "C1-3B-A", "C1-45-A", "C1-4F-A", "C3-9-A", "C3-13-A", "C3-1D-A", "C3-27-A", "C3-31-A", "C1-3A-A", "C1-44-A", "C1-4E-A", "C3-12-A", "C3-1C-A", "C3-26-A", "C3-30-A", "C1-39-A", "C1-43-A", "C1-4D-A", "C3-1B-A", "C3-25-A", "C3-2F-A", "C3-39-A", "C1-4C-A", "C3-24-A", "C3-2E-A" ]
   19.    Odd        Odd     Even      North  Vertex  Bottom-Top           A6-0-D      1        7           6        631         D4-1F8-D
                                                                           A6-0-D      1        5           4        76          C4-3C-D
                                                                           [ "C4-3C-D", "C4-45-D", "C4-3C-F", "C4-3C-E", "C4-3D-D", "C4-4E-D", "C4-45-F", "C4-45-E", "C4-46-D", "C4-3D-F", "C4-3D-E", "C4-3E-D", "C5-6-D", "C4-4E-F", "C4-4E-E", "C4-4F-D", "C4-46-F", "C4-46-E", "C4-47-D", "C4-3E-F", "C4-3E-E", "C6-3-D", "C5-6-E", "C5-7-D", "C4-4F-F", "C4-4F-E", "C4-50-D", "C4-47-F", "C4-47-E", "C6-2-D", "C6-2-E", "C5-10-D", "C5-7-F", "C5-7-E", "C5-8-D", "C4-50-F", "C4-50-E", "C6-1-D", "C6-1-E", "C6-2-F", "C6-C-D", "C5-10-E", "C5-11-D", "C5-8-F", "C5-8-E", "C6-0-D", "C6-0-E", "C6-1-F", "C6-B-D", "C6-B-E", "C5-1A-D", "C5-11-F", "C5-11-E", "C6-9-D", "C6-0-F", "C6-A-D", "C6-A-E", "C6-B-F", "C6-15-D", "C5-1A-E", "C6-12-D", "C6-9-F", "C6-9-E", "C6-A-F", "C6-14-D", "C6-14-E", "C6-1B-D", "C6-12-F", "C6-12-E", "C6-13-D", "C6-13-E", "C6-14-F", "C6-1E-D", "C6-1C-D", "C6-13-F", "C6-1D-D" ]
   20.    Odd        Odd     Even      South  Vertex  Bottom-Top           A5-0-D      1        7           6        631         D3-1F8-D
                                                                           A5-0-D      1        5           4        76          C3-3C-D
                                                                           [ "C3-3C-D", "C3-45-D", "C3-3C-F", "C3-3C-E", "C3-3D-D", "C3-4E-D", "C3-45-F", "C3-45-E", "C3-46-D", "C3-3D-F", "C3-3D-E", "C3-3E-D", "C5-1B-D", "C3-4E-F", "C3-4E-E", "C3-4F-D", "C3-46-F", "C3-46-E", "C3-47-D", "C3-3E-F", "C3-3E-E", "C4-36-D", "C5-12-F", "C5-12-D", "C3-4F-F", "C3-4F-E", "C3-50-D", "C3-47-F", "C3-47-E", "C4-3F-D", "C4-36-F", "C5-1C-D", "C5-12-E", "C5-9-F", "C5-9-D", "C3-50-F", "C3-50-E", "C4-48-D", "C4-3F-F", "C4-3F-E", "C4-40-D", "C5-13-F", "C5-13-D", "C5-9-E", "C5-0-F", "C5-0-D", "C4-48-F", "C4-48-E", "C4-49-D", "C4-40-F", "C5-1D-D", "C5-13-E", "C5-A-F", "C5-A-D", "C5-0-E", "C5-1-D", "C4-49-F", "C4-49-E", "C4-4A-D", "C5-14-F", "C5-14-D", "C5-A-E", "C5-1-F", "C5-1-E", "C5-2-D", "C4-4A-F", "C5-1E-D", "C5-14-E", "C5-B-F", "C5-B-D", "C5-2-F", "C5-2-E", "C5-3-D", "C5-15-D", "C5-B-E", "C5-C-D" ]
   Polar pentagons
   21.    Odd        Even    Odd       North  Edge    Left-Right*          A0-0-G      1        6           5        226         D2-12-A
                                                                           [ "D2-12-A", "D0-F2-A", "D0-D6-A", "D0-BA-A", "D0-9E-A", "D0-82-A", "D0-66-A", "D0-4A-A", "D0-2E-A", "D0-12-A", "D2-2E-A", "D2-13-A", "D0-D7-A", "D0-BB-A", "D0-9F-A", "D0-83-A", "D0-67-A", "D0-4B-A", "D0-2F-A", "D0-13-A", "D8-F2-A", "D2-4A-A", "D2-2F-A", "D2-14-A", "D0-BC-A", "D0-A0-A", "D0-84-A", "D0-68-A", "D0-4C-A", "D0-30-A", "D0-14-A", "D8-D7-A", "D8-D6-A", "D2-66-A", "D2-4B-A", "D2-30-A", "D2-15-A", "D0-A1-A", "D0-85-A", "D0-69-A", "D0-4D-A", "D0-31-A", "D0-15-A", "D8-BC-A", "D8-BB-A", "D8-BA-A", "D2-82-A", "D2-67-A", "D2-4C-A", "D2-31-A", "D2-16-A", "D0-86-A", "D0-6A-A", "D0-4E-A", "D0-32-A", "D0-16-A", "D8-A1-A", "D8-A0-A", "D8-9F-A", "D8-9E-A", "D2-9E-A", "D2-83-A", "D2-68-A", "D2-4D-A", "D2-32-A", "D2-17-A", "D0-6B-A", "D0-4F-A", "D0-33-A", "D0-17-A", "D8-86-A", "D8-85-A", "D8-84-A", "D8-83-A", "D8-82-A", "D2-BA-A", "D2-9F-A", "D2-84-A", "D2-69-A", "D2-4E-A", "D2-33-A", "D2-18-A", "D0-50-A", "D0-34-A", "D0-18-A", "D8-6B-A", "D8-6A-A", "D8-69-A", "D8-68-A", "D8-67-A", "D8-66-A", "D2-D6-A", "D2-BB-A", "D2-A0-A", "D2-85-A", "D2-6A-A", "D2-4F-A", "D2-34-A", "D2-19-A", "D0-35-A", "D0-19-A", "D8-50-A", "D8-4F-A", "D8-4E-A", "D8-4D-A", "D8-4C-A", "D8-4B-A", "D8-4A-A", "D2-F2-A", "D2-D7-A", "D2-BC-A", "D2-A1-A", "D2-86-A", "D2-6B-A", "D2-50-A", "D2-35-A", "D2-1A-A", "D0-1A-A", "D8-35-A", "D8-34-A", "D8-33-A", "D8-32-A", "D8-31-A", "D8-30-A", "D8-2F-A", "D8-2E-A", "D4-12-A", "D4-13-A", "D4-14-A", "D4-15-A", "D4-16-A", "D4-17-A", "D4-18-A", "D4-19-A", "D4-1A-A", "D0-1A-B", "D8-1A-A", "D8-19-A", "D8-18-A", "D8-17-A", "D8-16-A", "D8-15-A", "D8-14-A", "D8-13-A", "D8-12-A", "D4-2E-A", "D4-2F-A", "D4-30-A", "D4-31-A", "D4-32-A", "D4-33-A", "D4-34-A", "D4-35-A", "D6-1A-A", "D6-35-A", "D6-50-A", "D6-6B-A", "D6-86-A", "D6-A1-A", "D6-BC-A", "D6-D7-A", "D6-F2-A", "D4-4A-A", "D4-4B-A", "D4-4C-A", "D4-4D-A", "D4-4E-A", "D4-4F-A", "D4-50-A", "D6-19-A", "D6-34-A", "D6-4F-A", "D6-6A-A", "D6-85-A", "D6-A0-A", "D6-BB-A", "D6-D6-A", "D4-66-A", "D4-67-A", "D4-68-A", "D4-69-A", "D4-6A-A", "D4-6B-A", "D6-18-A", "D6-33-A", "D6-4E-A", "D6-69-A", "D6-84-A", "D6-9F-A", "D6-BA-A", "D4-82-A", "D4-83-A", "D4-84-A", "D4-85-A", "D4-86-A", "D6-17-A", "D6-32-A", "D6-4D-A", "D6-68-A", "D6-83-A", "D6-9E-A", "D4-9E-A", "D4-9F-A", "D4-A0-A", "D4-A1-A", "D6-16-A", "D6-31-A", "D6-4C-A", "D6-67-A", "D6-82-A", "D4-BA-A", "D4-BB-A", "D4-BC-A", "D6-15-A", "D6-30-A", "D6-4B-A", "D6-66-A", "D4-D6-A", "D4-D7-A", "D6-14-A", "D6-2F-A", "D6-4A-A", "D4-F2-A", "D6-13-A", "D6-2E-A", "D6-12-A" ]
                                                                           A0-0-G      1        4           3        31          C2-6-A
   22.    Odd        Even    Odd       South  Edge    Left-Right*          A9-0-H      1        6           5        226         D9-1E6-A
                                                                           A9-0-H      1        4           3        31          C9-36-A
   23.    Even       Odd     Odd       North  Edge    Bottom-Top*          A0-0-B      0        5           5        226         C0-21-D
                                                                           A0-0-B      0        3           3        31          B0-5-D
   24.    Even       Odd     Odd       South  Edge    Bottom-Top*          A9-0-C      0        5           5        226         C9-39-D
                                                                           A9-0-C      0        3           3        31          B9-7-D
   25.    Even       Even    Even      North  Vertex  Left-Right*          A0-0-B      0        6           6        631         D0-105-A
                                                                           A0-0-B      0        4           4         76         C0-21-A
                                                                           [ "C0-21-A", "C0-2C-A", "C0-22-A", "C0-18-A", "C0-E-A", "C2-E-A", "C2-5-A", "C0-23-A", "C0-19-A", "C0-F-A", "C0-5-A", "C8-2C-A", "C2-21-A", "C2-18-A", "C2-F-A", "C2-6-A", "C0-1A-A", "C0-10-A", "C0-6-A", "C8-23-A", "C8-22-A", "C8-21-A", "C2-22-A", "C2-19-A", "C2-10-A", "C2-7-A", "C0-11-A", "C0-7-A", "C8-1A-A", "C8-19-A", "C8-18-A", "C2-2C-A", "C2-23-A", "C2-1A-A", "C2-11-A", "C2-8-A", "C0-8-A", "C8-11-A", "C8-10-A", "C8-F-A", "C8-E-A", "C4-5-A", "C4-6-A", "C4-7-A", "C4-8-A", "C0-8-B", "C8-8-A", "C8-7-A", "C8-6-A", "C8-5-A", "C4-E-A", "C4-F-A", "C4-10-A", "C4-11-A", "C6-8-A", "C6-11-A", "C6-1A-A", "C6-23-A", "C6-2C-A", "C4-18-A", "C4-19-A", "C4-1A-A", "C6-7-A", "C6-10-A", "C6-19-A", "C6-22-A", "C4-21-A", "C4-22-A", "C4-23-A", "C6-6-A", "C6-F-A", "C6-18-A", "C6-21-A", "C4-2C-A", "C6-5-A", "C6-E-A" ]
                                                                           B0-2-B      2        6           4        76          D0-69-A
                                                                           [ "D0-69-A", "D0-86-A", "D0-6A-A", "D0-4E-A", "D0-32-A", "D2-32-A", "D2-17-A", "D0-6B-A", "D0-4F-A", "D0-33-A", "D0-17-A", "D8-86-A", "D2-69-A", "D2-4E-A", "D2-33-A", "D2-18-A", "D0-50-A", "D0-34-A", "D0-18-A", "D8-6B-A", "D8-6A-A", "D8-69-A", "D2-6A-A", "D2-4F-A", "D2-34-A", "D2-19-A", "D0-35-A", "D0-19-A", "D8-50-A", "D8-4F-A", "D8-4E-A", "D2-86-A", "D2-6B-A", "D2-50-A", "D2-35-A", "D2-1A-A", "D0-1A-A", "D8-35-A", "D8-34-A", "D8-33-A", "D8-32-A", "D4-17-A", "D4-18-A", "D4-19-A", "D4-1A-A", "D0-1A-B", "D8-1A-A", "D8-19-A", "D8-18-A", "D8-17-A", "D4-32-A", "D4-33-A", "D4-34-A", "D4-35-A", "D6-1A-A", "D6-35-A", "D6-50-A", "D6-6B-A", "D6-86-A", "D4-4E-A", "D4-4F-A", "D4-50-A", "D6-19-A", "D6-34-A", "D6-4F-A", "D6-6A-A", "D4-69-A", "D4-6A-A", "D4-6B-A", "D6-18-A", "D6-33-A", "D6-4E-A", "D6-69-A", "D4-86-A", "D6-17-A", "D6-32-A" ]
                                                                           B0-2-B      2        8           6        76          E0-321-A
                                                                           B0-2-B      2        10          8       5536         F0-1A79-A
   26.    Even       Even    Even      South  Vertex  Left-Right*          A9-0-C      0        6           6        631         D9-1EF-A
                                                                           A9-0-C      0        4           4        76          C9-39-A
   27.    Odd        Odd     Even      North  Vertex  Bottom-Top*          A0-0-G      1        7           6        631         D0-12-D
                                                                           A0-0-G      1        5           4        76          C0-6-D
   28.    Odd        Odd     Even      South  Vertex  Bottom-Top*          A9-0-H      1        7           6        631         D1-1E6-D
                                                                           A9-0-H      1        5           4        76          C1-36-D

   * There is no obvious definition of left/right/bottom/top for the polar cases.
     All scanlines are defined in a clockwise manner when looking at the zones on the globe.
*/

#define POW3(x) ((x) < sizeof(powersOf3) / sizeof(powersOf3[0]) ? (uint64)powersOf3[x] : (uint64)pow(3, x))

void getI3HFirstSubZoneCentroid(I3HZone zone, int rDepth, Pointd c)
{
   Pointd vertices[6];
   __attribute__((unused)) int nv = zone.getVertices(vertices);
   int subHex = zone.subHex, levelI9R = zone.levelI9R;
   uint64 rhombusIX = zone.rhombusIX;
   bool oddDepth = rDepth & 1, oddParent = subHex > 2;
   bool southRhombus = zone.rootRhombus & 1;

   if(oddParent)
   {
      // Odd Level parents -- e.g., A6-0-E (level 1)
      if(oddDepth)
      {
         // e.g., A6-0-E (level 1) with level 4 sub-zones (relative depth: 3)          -- first zone: C6-6-A
         // e.g., A6-0-D (level 1 pentagon) with level 4 sub-zones (relative depth: 3) -- first zone: C6-3-A
         // e.g., A5-0-D (level 1 pentagon) with level 4 sub-zones (relative depth: 3) -- first zone: C4-36-A

         // (in ISEA Planar projection) Left-to-Right Scanlines; Top to Bottom Scanline order
         // Start from hexagon / pentagon edge
#if 1 //def _DEBUG
         int tl = -1, i;
         for(i = 0; i < nv; i++)
         {
            if(tl == -1 || vertices[i].y < vertices[tl].y ||
               (fabs(vertices[i].y - vertices[tl].y) < 1E-11 && vertices[i].x > vertices[tl].x))
               tl = i;
         }
// #else
         if(subHex == 6)
            tl = 0;
         else if(nv == 5)
            tl = southRhombus ? (subHex == 7 ? 1 : 4) : 3;
#endif
         c = vertices[tl];

         // NOTE: This is the vertex immediately to the right of the odd parent / even depth
      }
      else
      {
         // e.g., A6-0-E (level 1) with level 7 sub-zones (relative depth: 6)          -- first zone: D6-9-D
         // e.g., A6-0-D (level 1 pentagon) with level 7 sub-zones (relative depth: 6) -- first zone: D4-1F8-D
         // e.g., A5-0-D (level 1 pentagon) with level 7 sub-zones (relative depth: 6) -- first zone: D3-1F8-D

         // (in ISEA Planar projection) Bottom-to-Top Scanlines; Left to Right Scanline order
         // Start from hexagon / pentagon vertex -- leftmost vertex in planar ISEA (top-left corner in ISEA 5x6)
#if 1 //0
         int tl = -1, i;
         for(i = 0; i < nv; i++)
         {
            if(tl == -1 || vertices[i].y < vertices[tl].y ||
               (fabs(vertices[i].y - vertices[tl].y) < 1E-11 && vertices[i].x < vertices[tl].x))
               tl = i;
         }
//#else
         // int tl = subHex == 6 ? 4 : (nv == 5) ? 3 : 0;
         if(subHex == 6)
            tl = 4;
         else if(nv == 5)
            tl = subHex == 7 ? 0 : 3;
#endif
         c = vertices[tl];
      }
   }
   else
   {
      // Even Level parents -- e.g., B6-5-A (level 2)
      if(oddDepth)
      {
         // e.g., B6-5-A (level 2) with level 5 sub-zones (relative depth: 3)          -- first zone: C6-16-D
         // e.g., A2-0-A (level 0 pentagon) with level 3 sub-zones (relative depth: 3) -- first zone: B0-7-D
         // e.g., A3-0-A (level 0 pentagon) with level 3 sub-zones (relative depth: 3) -- first zone: B1-7-D

         // (in ISEA Planar projection) Bottom-to-Top Scanlines; Left to Right Scanline order
         // Start from hexagon / pentagon edge
#if 1 //0
         int left = -1, i;
         for(i = 0; i < nv; i++)
            if(left == -1 || vertices[i].x < vertices[left].x) left = i;

//#else
         //int left = subHex == 1 ? 0 : (nv == 6 || !(rootRhombus & 1)) ? 3 : 2;
         if(subHex == 1)
            left = 0;
         else if(nv == 5 && southRhombus)
            left = subHex == 2 ? 4 : 2;
#endif
         c = vertices[left];
      }
      else
      {
         // e.g., B6-5-A (level 2) with level 8 sub-zones (relative depth: 6)          -- first zone: E6-5F1-A
         // e.g., A2-0-A (level 0 north pentagon) with level 6 sub-zones (relative depth: 6) -- first zone: D0-105-A
         // e.g., A3-0-A (level 0 south pentagon) with level 6 sub-zones (relative depth: 6) -- first zone: D2-1EF-A

         // (in ISEA Planar projection) Left-to-Right Scanlines; Bottom to Top Scanline order
         // Start from hexagon / pentagon vertex
         if(nv == 5 && subHex != 1 && subHex != 2 && !southRhombus)
         {
            int top = 4;
            c = vertices[top];
            /*
            int depthO2 = rDepth/2, pow3DepthO2 = (int)POW3(depthO2);
            int xFromTop = (int)(pow3DepthO2 / 3 - 1); // https://oeis.org/A024023 where n = depthO2
            int nAboveRows = xFromTop / 2;             // https://oeis.org/A003462 where n = depthO2
            double f = 1.0 / pow3DepthO2;
            c.x = vertices[top].x + xFromTop * f;
            c.y = vertices[top].y + nAboveRows * f;
            // 2: top + { 0/3, 0/3 };         4: top + { 2/9, 1/9 };       6: top + { 8/27, 4/27 };
            // 8: top + { 26/81, 13/81 };    10: top + { 80/243, 40/243 }
            */
         }
         else
         {
            // The first vertex is between the topmost and rightmost vertices in rotated ISEA 5x6 space
#if 0
            int top = -1, right = -1, ix = -1, i;
            for(i = 0; i < nv; i++)
            {
               if(top   == -1 || vertices[i].y < vertices[top  ].y) top   = i;
               if(right == -1 || vertices[i].x > vertices[right].x) right = i;
            }
            if(right - top == 2)             ix = top + 1;
            else if(top - right == 2)        ix = right + 1;
            else if(top == 0)                ix = nv-1;
            else if(right == 0)              ix = nv-1;
            else if(top == 1 || right == 1)  ix = 0;
#else
            int ix = subHex == 1 ? 0 : (nv == 6) ? 5 : 4;
            int divs = (int)POW3(levelI9R);
            bool edgeHex = nv == 6 && rhombusIX && (southRhombus ? ((rhombusIX % divs) == 0) : ((rhombusIX / divs) == 0));
            if(edgeHex && southRhombus)
               ix = 4;
#endif
            c = vertices[ix];
         }
      }
   }

   // getVertices() currently return negative vertices...
   if(c.x > 5 && c.y > 5)
      c.x -= 5, c.y -= 5;
   else if(c.x < 0 && c.y < 1)
      c.x += 5, c.y += 5;
}

// This function handles variations #1 (basic), #5 (north edge hex), #6 (south edge hex), #13 (north pentagon), #14 (south pentagon), #21 (north pole) and #22 (south pole)
static int64 generateOddParentOddDepth(void * context, bool (* centroidCallback)(void * context, uint64 index, Pointd centroid),
   Pointd firstCentroid, int rDepth, double u,
   int nv, bool polarPentagon, bool southRhombus, bool edgeHex, int64 index)
{
   bool keepGoing = true;
   Pointd centroid;
   bool northPentagon = nv == 5 && !southRhombus;
   bool southPentagon = nv == 5 && southRhombus;
   // Start from hexagon / pentagon edge
   // Left-To-Right Scanlines

   // e.g., A6-0-E (level 1) with level 4 sub-zones (relative depth: 3)          -- first zone: C6-6-A
   /*    rd: 3    nRows = 7   maxCols = 7
        x x x x      4
       x x x x x      5
      x x x x x x      6
     x x x x x x x      7
      x x x x x x      6
       x x x x x      5
        x x x x      4
   */
   // e.g., A5-0-D (level 1 south pentagon) with level 4 sub-zones (relative depth: 3) -- first zone: C4-36-A
   // e.g., A6-0-D (level 1 north pentagon) with level 4 sub-zones (relative depth: 3) -- first zone: C6-3-A
   int nHalfRows = (int)POW3((rDepth-1)/2);
   int nRows = 2 * nHalfRows + 1, maxCols = nRows;
   int minCols = maxCols - (maxCols-1)/2;
   int r, nCols, col;
   Pointd rc;
   int64 i = 0;

   // First half
   for(r = 0, nCols = minCols; keepGoing && r <= nHalfRows; r++, nCols++)
   {
      if(index != -1 && i + nCols <= index)
      {
         i += nCols;
         continue;
      }

      // Computing start of scanline
      if(polarPentagon)
         move5x6Vertex(rc, firstCentroid, (southPentagon ? -r : r) * u, (southPentagon ? -r : r) * u);
      else if(northPentagon || (edgeHex && !southRhombus))
         move5x6Vertex(rc, firstCentroid, 0, r * u);
      else
         move5x6Vertex(rc, firstCentroid, -r * u, 0);

      // Iterating through scanline
      for(col = 0; keepGoing && col < nCols; col++)
      {
         if(index != -1)
         {
            // Jump to index
            col = (int)(index - i);
            i = index;
         }

         if(polarPentagon)
         {
            int a = Min(col, r), b = col - a;
            Pointd t;

            // REVIEW: Using move5x6Vertex() breaks things here
            /*(b ? move5x6Vertex : */move5x6Vertex3/*)*/(t, rc, 0, (southPentagon ? a : -a) * u);

            if(b)
            {
               int maxB = nCols - 1 - 2*r, c = 0;
               Pointd i2;

               if(b > maxB)
                  c = b - maxB, b = maxB;

               cross5x6Interruption(  t, i2, southPentagon, northPentagon || r >= nHalfRows);
               (c ? move5x6Vertex : move5x6Vertex3)(centroid, i2, (southPentagon ? b : -b) * u, (southPentagon ? b : -b) * u);

               if(c)
               {
                  cross5x6Interruption(centroid, i2, southPentagon, northPentagon || r >= nHalfRows);
                  move5x6Vertex3(centroid, i2,
                     southPentagon ? (r >= nHalfRows ? 0 : c * u) : -c * u,
                     southPentagon && r >= nHalfRows ? -c * u : 0);
               }
            }
            else
               centroid = t;
         }
         else if(edgeHex || northPentagon)
         {
            int a = Min(col, nHalfRows), b = col - a;
            Pointd t;
            (b ? move5x6Vertex : move5x6Vertex3)(t, rc, a * u, southRhombus ? a * u : 0);

            if(b)
            {
               Pointd i2;
               cross5x6Interruption(t, i2, southRhombus, false);
               move5x6Vertex3(centroid, i2, b * u, southRhombus ? 0 : b * u);
            }
            else
               centroid = t;
         }
         else
            move5x6Vertex(centroid, rc, col * u, col * u);//, false);

         if((keepGoing = centroidCallback(context, i, centroid))) i++;
      }
   }

   // Second half
   for(r = 1, nCols = maxCols-1; keepGoing && r <= nHalfRows; r++, nCols--)
   {
      int n = -1;

      // Computing start of scanline
      if(northPentagon || polarPentagon)
         nCols--;
      else if(southPentagon)
         n = (nCols-r)/2;

      if(index != -1 && i + nCols <= index);  // nCols is not always the actual number of indices
      else if(polarPentagon)
         move5x6Vertex(rc, firstCentroid, southPentagon ? (-1 - r * u) : (1 + r * u), southPentagon ? (-1 -r * u) : (1 + r * u));
      else if(northPentagon || (edgeHex && !southRhombus))
         move5x6Vertex(rc, firstCentroid, r * u, (r + nHalfRows) * u);
      else
         move5x6Vertex(rc, firstCentroid, -nHalfRows * u, r * u);

      // Iterating through scanline
      for(col = 0; keepGoing && col < nCols; col++)
      {
         if(index == -1 || index == i)
         {
            if(northPentagon || edgeHex || polarPentagon)
            {
               int a = Min(col, nHalfRows - r), b = col - a;
               Pointd t;
               if(b)
               {
                  if(polarPentagon || edgeHex)
                  {
                     Pointd i2;
                     move5x6Vertex(t, rc, (polarPentagon && southPentagon ? -a : a) * u, southRhombus && !polarPentagon ? a * u : 0);
                     cross5x6Interruption(t, i2, southRhombus, southPentagon && polarPentagon);
                     t = i2;
                  }
                  else
                     move5x6Vertex3(t, rc, (polarPentagon && southPentagon ? -a : a) * u, southRhombus && !polarPentagon ? a * u : 0);
                  move5x6Vertex(centroid, t, polarPentagon ? 0 : b * u, southRhombus ? (polarPentagon ? -b * u : 0) : b * u);
               }
               else
                  move5x6Vertex3(centroid, rc, (polarPentagon && southPentagon ? -a : a) * u, southRhombus && !polarPentagon ? a * u : 0);
            }
            else
               move5x6Vertex(centroid, rc, col * u, col * u);//, false);

            if((keepGoing = centroidCallback(context, i, centroid))) i++;
         }
         else if(nv == 5 && n > col && n - col < index - i)
         {
            i += (int64)n - col + 1;
            col = n;
         }
         else
         {
            int ff = (int)Min(index - i, nCols - col);
            col += ff-1;
            i += ff;
         }

         if(col == n)
            col = nCols - 1 - n; // Skip interruption
      }
   }

#ifdef _DEBUG
   if(keepGoing && i != ((Array)context).count)
   {
      PrintLn("WARNING: Mismatched sub-zone index");
   }
#endif
   return keepGoing ? -1 : i;
}

// This function handles variations #2 (basic), #7 (north edge hex), #8 (south edge hex), #15 (north pentagon), #16 (south pentagon), #23 (north pole) and #24 (south pole)
static int64 generateEvenParentOddDepth(void * context, bool (* centroidCallback)(void * context, uint64 index, Pointd centroid),
   Pointd firstCentroid, int rDepth, double u,
   int nv, bool polarPentagon, bool southRhombus, bool edgeHex, int64 index)
{
   // Start from hexagon / pentagon edge

   // Bottom-To-Top Scanlines ("rows" are the vertical scanlines)
   // e.g., B6-5-A (level 2) with 37 level 5 sub-zones (relative depth: 3)     -- first zone: C6-16-D
   // e.g., A2-0-A (level 0 north pentagon) with level 3 sub-zones (relative depth: 3) -- first zone: B0-7-D
   // e.g., A3-0-A (level 0 south pentagon) with level 3 sub-zones (relative depth: 3) -- first zone: B1-7-D

   // North Edge Hex: http://localhost:8080/ogcapi/dggs/ISEA3H/zones?parent-zone=B6-1-A&f=json&compact-zones=false&zone-level=7&f=geojson&crs=OGC:1534
   // South Edge Hex: http://localhost:8080/ogcapi/dggs/ISEA3H/zones?parent-zone=B5-3-A&f=json&compact-zones=false&zone-level=7&f=geojson&crs=OGC:1534
   /*                   nRows = 7      maxCols = 7
             7
           6   6
         5       5
       4           4
        1    x
      2    x   x
    3    x   x   x
  4    x   x   x   x
    3    x   x   x
  4    x   x   x   x
    3    x   x   x
  4    x   x   x   x
    3    x   x   x
  4    x   x   x   x
    3    x   x   x
      2    x   x
        1    x
   */
   bool keepGoing = true;
   Pointd centroid;
   bool northPentagon = nv == 5 && !southRhombus;
   bool southPentagon = nv == 5 && southRhombus;
   int nHalfRows = (int)POW3((rDepth-1)/2);
   int nRows = 2 * nHalfRows + 1, maxCols = nRows;
   int minCols = maxCols - (maxCols-1)/2;
   int r, nCols, col;
   Pointd rc;
   int64 i = 0;

   // First half
   for(r = 0, nCols = minCols; keepGoing && r <= nHalfRows; r++, nCols++)
   {
      if(index != -1 && i + nCols <= index)
      {
         i += nCols;
         continue;
      }

      // Computing start of scanline
      if(polarPentagon)
      {
         if(r > nHalfRows / 2)
         {
            int a = nHalfRows/2, b = r - (a + 1);
            Pointd i1;

            // REVIEW: Avoid calling move5x6Vertex3() before cross5x6Interruption()
            move5x6Vertex(rc, firstCentroid, (a * 2 + 1) * (southPentagon ?-1:1) * u / 3, (a * 1 + 1) * (southPentagon ?-1:1) * u / 3);
            cross5x6Interruption(rc, i1, southPentagon, southPentagon);
            move5x6Vertex3(rc, i1, (1 + b * 1) * (southPentagon ?-1:1) * u / 3, (1 + b * 2) * (southPentagon ?-1:1) * u / 3);
         }
         else
            move5x6Vertex(rc, firstCentroid, r * (southPentagon ?-2:2) * u / 3, r * (southPentagon ?-1:1) * u / 3);
      }
      else if(southRhombus && (edgeHex || southPentagon) && r > nHalfRows / 2)
      {
         int ix = (int)(firstCentroid.x - 1E-11);
         int iy = (int)(firstCentroid.y - 1E-11);
         Pointd i0 { firstCentroid.x + ((iy + 1) - firstCentroid.y), iy + 1 };
         Pointd i1 { i0.y - 1, iy + 1 + (ix + 1 - i0.x) };
         int b = r - nHalfRows / 2 - 1;

         move5x6Vertex3(rc, i1, (b * 2 + 1) * u / 3, ((nHalfRows + 1)/2 + b) * u / 3);

         /*
            B5-3-A

            c:  { 1.4444444444444, 2.8888888888889 }
            i1: { 2, 3.4444444444444 }

            SZ Level  nHalfRows     u                          r     rc

            3         1             0.33333333333333331        1     { 2.1111111111111, 3.5555555555556 }
            5         3             0.1111111111111111         2     { 2.037037037037,  3.5185185185185 }
            7         9             0.037037037037037035       5     { 2.0123456790123, 3.5061728395062 }
            9         27            0.012345679012345678       14    { 2.0041152263374, 3.5020576131687 }
         */
      }
      else
         move5x6Vertex(rc, firstCentroid, r * u / 3, r * 2 * u / 3);

      // Iterating through scanline
      for(col = 0; keepGoing && col < nCols; col++)
      {
         if(index != -1)
         {
            col = (int)(index - i);
            i = index;
         }

         if(polarPentagon)
         {
            Pointd t, start;
            int colRem = col;
            int n;
            bool crossingLeft = southPentagon ? r >= nHalfRows : r < nHalfRows;

            if(r > nHalfRows / 2)
            {
               int a;
               n = (r - nHalfRows/2) * 2 - 1;
               a = Min(col, n);
               colRem = col - a;

               // REVIEW: Avoid calling move5x6Vertex3() before cross5x6Interruption()
               (colRem ? move5x6Vertex : move5x6Vertex3)(start, rc, a * (southPentagon ?-1:1) * u/3, -1 * a * (southPentagon ?-1:1) * u/3);
               if(colRem)
               {
                  n = (nHalfRows-r)/2 + (nHalfRows-r);
                  cross5x6Interruption(start, t, southPentagon, crossingLeft);
                  start = t;
               }
            }
            else
            {
               start = rc;
               n = (nHalfRows + r) / 2;
            }

            if(colRem)
            {
               int a = Min(colRem, n), b = colRem - a;
               Pointd i2;

               (b ? move5x6Vertex : move5x6Vertex3)(t, start, -a * (southPentagon ?-1:1) * u/3, -2*a * (southPentagon ?-1:1) * u/3);
               if(b)
               {
                  bool oddRow = r&1;
                  if(!oddRow)
                  {
                     b--;
                     t.x -= (southPentagon ?-1:1) * u/3, t.y -= (southPentagon ?-1:1) * u/3;
                  }
                  cross5x6Interruption(t, i2, southPentagon, crossingLeft);
                  if(!oddRow)
                     i2.x -= (southPentagon ?-1:1) * u/3, i2.y -= (southPentagon ?-1:1) * u/3;

                  if(b > n)
                  {
                     a = Min(b, n);
                     b -= a;
                     (a ? move5x6Vertex : move5x6Vertex3)(t, i2, a * (southPentagon ?-1:1) * -2*u/3, -a * (southPentagon ?-1:1) * u/3);
                     if(a)
                        cross5x6Interruption(t, i2, southPentagon, crossingLeft);
                     else
                        i2 = t;
                     move5x6Vertex3(centroid, i2, -b * (southPentagon ?-1:1) * u/3, b * (southPentagon ?-1:1) * u/3);
                  }
                  else
                     move5x6Vertex3(centroid, i2, b * -2* (southPentagon ?-1:1) * u/3, -b * (southPentagon ?-1:1) * u/3);
               }
               else
                  centroid = t;
            }
            else
               centroid = start;

         }
         else if(southRhombus && (edgeHex || southPentagon) && r > nHalfRows / 2)
         {
            int n = (r - nHalfRows/2) * 2 - 1;
            int a = Min(col, n), b = col - a;
            Pointd t;

            if(b)
            {
               Pointd i2;
               if(!southPentagon || r < nHalfRows) // REVIEW: crossing when at intersection corner
               {
                  move5x6Vertex(t, rc, -a * u/3, -2 * a * u/3);
                  cross5x6Interruption(t, i2, true, true);
               }
               else
                  move5x6Vertex3(i2, rc, -a * u/3, -2 * a * u/3);
               move5x6Vertex3(centroid, i2, b * u/3, -b * u/3);
            }
            else
               move5x6Vertex3(centroid, rc, -a * u/3, -2 * a * u/3);
         }
         else if(northPentagon && r > nHalfRows / 2)
         {
            int n = (2*nHalfRows - r);
            int a = Min(col, n), b = col - a;
            Pointd t;

            (b ? move5x6Vertex : move5x6Vertex3)(t, rc, a * u/3, -a * u/3);
            if(b)
            {
               Pointd i2;
               cross5x6Interruption(t, i2, false, false);
               move5x6Vertex3(centroid, i2, b * 2*u/3, b * u/3);
            }
            else
               centroid = t;
         }
         else
            move5x6Vertex3(centroid, rc, col * u/3, -col * u/3);

         if((keepGoing = centroidCallback(context, i, centroid))) i++;
      }
   }

   // Second half
   for(r = 1, nCols = maxCols-1; keepGoing && r <= nHalfRows; r++, nCols--)
   {
      int skip = nv == 5 ? r : 0;
      bool crosses = false;

      if(index != -1 && i + nCols - skip <= index)
      {
         i += nCols - skip;
         continue;
      }

      // Computing start of scanline
      if(polarPentagon)
      {
         int a = nHalfRows/2, b = nHalfRows - (a + 1), aa = Min(r, nHalfRows/2);
         Pointd i1;

         move5x6Vertex(rc, firstCentroid, (a * 2 + 1) * (southPentagon ?-1:1) * u / 3, (a + 1) * (southPentagon ?-1:1) * u / 3);
         cross5x6Interruption(rc, i1, southPentagon, southPentagon);
         move5x6Vertex3(rc, i1, (1 + b + aa * 2) * (southPentagon ?-1:1) * u / 3, (1 + b * 2 + aa) * (southPentagon ?-1:1) * u / 3);
         if(r > nHalfRows/2)
         {
            int bb = (r > nHalfRows/2) ? r - (aa + 1) : 0;
            rc.x += (southPentagon ?-1:1) * u / 3, rc.y += (southPentagon ?-1:1) * u / 3;
            cross5x6Interruption(rc, i1, southPentagon, southPentagon);
            move5x6Vertex3(rc, i1, (1 + bb) * (southPentagon ?-1:1) * u / 3, (1 + bb * 2) * (southPentagon ?-1:1) * u / 3);
         }
      }
      else if(southRhombus && (edgeHex || southPentagon))
      {
         int ix = (int)(firstCentroid.x - 1E-11);
         int iy = (int)(firstCentroid.y - 1E-11);
         Pointd i0 { firstCentroid.x + ((iy + 1) - firstCentroid.y), iy + 1 };
         Pointd i1 { i0.y - 1, iy + 1 + (ix + 1 - i0.x) };
         int b = nHalfRows - nHalfRows / 2 - 1;

         move5x6Vertex3(rc, i1, u / 3 + b * 2 * u / 3 + r * u / 3, (nHalfRows + 1)/2.0 * u / 3 + b * u / 3 - r * u / 3);
      }
      else if(!southRhombus && edgeHex && r > nHalfRows / 2)
      {
         int i = r - nHalfRows/2 - 1;
         Pointd ii { firstCentroid.x + nHalfRows * 2 * u / 3, firstCentroid.y + nHalfRows * 2 * u / 3 };
         int iy = (int)(ii.y - 1E-11);
         Pointd t { iy + 2 - (ii.y - iy), ii.x };

         move5x6Vertex(rc, t,
            -(nHalfRows - 1)/2.0 * u / 3 + i * u / 3,
            u / 3 + i * 2 * u / 3
         );

         crosses = true;
      }
      else
      {
         double dx = nHalfRows *     u / 3 + r * 2 * u / 3;
         double dy = nHalfRows * 2 * u / 3 + r     * u / 3;
         move5x6Vertex(rc, firstCentroid, dx, dy);
      }

      // Iterating through scanline
      for(col = 0; keepGoing && col < nCols - skip; col++)
      {
         if(index != -1)
         {
            col = (int)(index - i);
            i = index;
         }

         if(polarPentagon)
         {
            int a, b = col;
            Pointd t, i2;
            int n = nHalfRows - 2*r;

            if(r <= nHalfRows / 2)
            {
               a = Min(col, n);
               b = col - a;

               (b ? move5x6Vertex : move5x6Vertex3)(t, rc, (southPentagon ? -1 : 1) * a * u/3, (southPentagon ? -1 : 1) *  -a * u/3);
               if(b)
               {
                  cross5x6Interruption(t, i2, southPentagon, southPentagon);
                  t = i2;
                  n = r;
               }
            }
            else
            {
               n = nHalfRows - r;
               t = rc;
            }

            if(b)
            {
               if(b > n)
               {
                  a = Min(b, n);
                  b -= a;
                  move5x6Vertex3(i2, t, (southPentagon ? -1 : 1) * a * 2*u/3, (southPentagon ? -1 : 1) * a * u/3);

                  if(b > n)
                  {
                     a = Min(b, n);
                     b -= a;
                     // REVIEW: move5x6Vertex3() currently crosses on arrival, so should not be used followed by cross5x6Interruption() ?
                     move5x6Vertex(t, i2, (southPentagon ? -1 : 1) * a * u/3, (southPentagon ? -1 : 1) * a * 2*u/3);
                     cross5x6Interruption(t, i2, southPentagon, southPentagon);

                     move5x6Vertex3(centroid, i2, (southPentagon ? -1 : 1) * -b * u/3, (southPentagon ? -1 : 1) * b * u/3);
                  }
                  else
                     move5x6Vertex3(centroid, i2, (southPentagon ? -1 : 1) * b * u/3, (southPentagon ? -1 : 1) * b * 2*u/3);
               }
               else
                  move5x6Vertex3(centroid, t, (southPentagon ? -1 : 1) * b * 2*u/3, (southPentagon ? -1 : 1) * b * u/3);
            }
            else
               centroid = t;
         }
         else if(crosses)
            move5x6Vertex(centroid, rc, col * 2*u/3, col * u/3);
         else if(southRhombus && (edgeHex || southPentagon))
         {
            int n = southPentagon ? nHalfRows - r : nCols - (nHalfRows - 2*r)-1;
            int a = Min(col, n), b = Max(0, col - n);
            Pointd t;

            if(b)
            {
               Pointd i2;
               if(southPentagon)
                  move5x6Vertex3(i2, rc, -a * u/3, -2 * a * u/3);
               else
               {
                  move5x6Vertex(t, rc, -a * u/3, -2 * a * u/3);
                  cross5x6Interruption(t, i2, true, true);
               }
               move5x6Vertex3(centroid, i2, b * u/3, -b * u/3);
            }
            else
               move5x6Vertex3(centroid, rc, -a * u/3, -2 * a * u/3);
         }
         else if(northPentagon)
         {
            int n = nHalfRows - r;
            int a = Min(col, n), b = col - a;
            Pointd t;

            move5x6Vertex3(t, rc, a * u/3, -a * u/3);
            if(b)
               move5x6Vertex3(centroid, t, b * 2*u/3, b * u/3);
            else
               centroid = t;
         }
         else
            move5x6Vertex3(centroid, rc, col * u/3, -col * u/3);

         if((keepGoing = centroidCallback(context, i, centroid))) i++;
      }
   }

#ifdef _DEBUG
   if(keepGoing && i != ((Array)context).count)
   {
      PrintLn("WARNING: Mismatched sub-zone index");
   }
#endif
   return keepGoing ? -1 : i;
}

// This function handles variations #3 (basic), #9 (north edge hex), #10 (south edge hex), #17 (north pentagon), #18 (south pentagon), #25 (north pole) and #26 (south pole)
static int64 generateEvenParentEvenDepth(void * context, bool (* centroidCallback)(void * context, uint64 index, Pointd centroid),
   Pointd firstCentroid, int rDepth, double u,
   int nv, bool polarPentagon, bool southRhombus, bool edgeHex, int64 index)
{
   // Start from hexagon / pentagon vertex
   // Left-To-Right Scanlines
   // e.g., B6-5-A (level 2) with level 4 sub-zones (relative depth: 2)          -- first zone: C6-19-A
   // e.g., A2-0-A (level 0 north pentagon) with level 6 sub-zones (relative depth: 6) -- first zone: D0-105-A
   // e.g., B2-0-A (level 0 north pentagon) with level 6 sub-zones (relative depth: 4) -- first zone: D0-24F-A
   // e.g., A3-0-A (level 0 south pentagon) with level 6 sub-zones (relative depth: 6) -- first zone: D2-1EF-A
   // e.g., A3-0-A (level 0 south pentagon) with level 4 sub-zones (relative depth: 4) -- first zone: C2-39-A
   /*
          x          1
    x   x   x   x       4
      x   x   x       3
    x   x   x   x       4
          x          1
   */
   bool keepGoing = true;
   Pointd centroid;
   bool southPentagon = nv == 5 && southRhombus;
   int nCapRows = (int)POW3((rDepth-2)/2), nMidRows = 2 * nCapRows + 1;
   int endCapSkip = nv == 5 ? (nCapRows + 1) / 2 : 0;
   int minCols = 1;
   int r, nCols, col;
   Pointd rc;
   int64 i = 0;

   // First cap
   for(r = 0, nCols = minCols; keepGoing && r < nCapRows; r++, nCols += 3)
   {
      if(index != -1 && i + nCols <= index)
      {
         i += nCols;
         continue;
      }

      // Computing start of scanline
      if(polarPentagon && southRhombus)
      {
         if(r > nCapRows / 2)
         {
            Pointd t, i2;
            move5x6Vertex(t, firstCentroid, -nCapRows * u, -(nCapRows - r) * u);
            cross5x6Interruption(t, i2, true, true);
            move5x6Vertex(rc, i2, 0, -(2*(r - nCapRows/2)-1) * u);
         }
         else
            move5x6Vertex3(rc, firstCentroid, -2*r * u, -r * u);
      }
      else if((edgeHex || nv == 5) && !southRhombus)
      {
         if(polarPentagon)
         {
            if(r > nCapRows / 2)
            {
               Pointd t, i2;
               move5x6Vertex(t, firstCentroid, nCapRows * u, (nCapRows - r) * u);
               cross5x6Interruption(t, i2, false, false);
               move5x6Vertex(rc, i2, 0, (2*(r - nCapRows/2)-1) * u);
            }
            else
               move5x6Vertex(rc, firstCentroid, 2*r * u, r * u);
         }
         else
            move5x6Vertex(rc, firstCentroid, -r * u, r * u);
      }
      else
         move5x6Vertex(rc, firstCentroid, r * -2 * u, r * -1 * u);

      // Iterating through scanline
      for(col = 0; keepGoing && col < nCols; col++)
      {
         if(index != -1)
         {
            col = (int)(index - i);
            i = index;
         }

         if((edgeHex || nv == 5) && !southRhombus)
         {
            if(r > nCapRows / 2)
            {
               int n = polarPentagon ? 2 * r - nCapRows : nCapRows + r;
               int a = Min(col, n), b = col - a;
               Pointd t;

               // REVIEW: Avoid calling move5x6Vertex3() before cross5x6Interruption()
               if(polarPentagon) // move5x6Vertex() breaks things here
                  (b ? move5x6Vertex : move5x6Vertex3)(t, rc, 0, -a * u);
               else
                  (b ? move5x6Vertex : move5x6Vertex3)(t, rc, a * u, 0);
               if(b)
               {
                  Pointd i2;
                  cross5x6Interruption(t, i2, false, polarPentagon);
                  if(polarPentagon)
                  {
                     int nb = 2*nCapRows - r, c = col - (a + (b = Min(b, nb)));
                     Pointd t2;

                     (c ? move5x6Vertex : move5x6Vertex3)(t2, i2, -b * u, -b * u);
                     if(c)
                     {
                        cross5x6Interruption(t2, i2, false, true);
                        move5x6Vertex3(centroid, i2, -c * u, 0);
                     }
                     else
                        centroid = t2;
                  }
                  else
                     move5x6Vertex3(centroid, i2, b * u, b * u);
               }
               else
                  centroid = t;
            }
            else if(polarPentagon)
               move5x6Vertex(centroid, rc, -col * u, -col * u);
            else
               move5x6Vertex(centroid, rc, col * u, 0);
         }
         else if((edgeHex || nv == 5) && southRhombus)
         {
            if(r > nCapRows / 2)
            {
               int n = polarPentagon ? 2 * r - nCapRows : nCapRows + r;
               int a = Min(col, n), b = col - a;
               Pointd t;

               if(polarPentagon)
                  (b ? move5x6Vertex : move5x6Vertex3)(t, rc, 0, a * u);
               else
                  (b ? move5x6Vertex : move5x6Vertex3)(t, rc, a * u, a * u);
               if(b)
               {
                  Pointd i2;
                  cross5x6Interruption(t, i2, true, false);

                  if(polarPentagon)
                  {
                     int nb = 2*nCapRows - r, c = col - (a + (b = Min(b, nb)));
                     Pointd t2;

                     // REVIEW: Avoid calling move5x6Vertex3() before cross5x6Interruption()
                     (c ? move5x6Vertex : move5x6Vertex3)(t2, i2, b * u, b * u);
                     if(c)
                     {
                        cross5x6Interruption(t2, i2, true, false);
                        move5x6Vertex3(centroid, i2, c * u, 0);
                     }
                     else
                        centroid = t2;
                  }
                  else
                     move5x6Vertex3(centroid, nv == 5 ? t : i2, b * u, nv == 5 ? b * u : 0);
               }
               else
                  centroid = t;
            }
            else
               move5x6Vertex(centroid, rc, col * u, col * u);
         }
         else
            move5x6Vertex(centroid, rc, col * u, col * u);

         if((keepGoing = centroidCallback(context, i, centroid))) i++;
      }
   }

   // Main portion
   for(r = 0; keepGoing && r < nMidRows; r++, nCols += (nCols & 1) ? 1 : -1)
   {
      int colSkip = (nv == 5 && r > nMidRows/2) ? r - nMidRows/2: 0;

      if(index != -1 && i + nCols - colSkip <= index)
      {
         i += nCols - colSkip;
         continue;
      }

      // Computing start of scanline
      if(polarPentagon && southRhombus)
      {
         Pointd t, i2, t2;
         int a = Min(r, nCapRows), b = r - a;

         move5x6Vertex(t, firstCentroid, -nCapRows * u, 0);
         cross5x6Interruption(t, i2, true, true);
         move5x6Vertex(t2, i2, -a * u, -(a/2 + nCapRows) * u);
         if(b)
         {
            cross5x6Interruption(t2, i2, true, true);
            move5x6Vertex(rc, i2, -(b/2) * u, -b * u);
         }
         else
            rc = t2;
      }
      else if((edgeHex || nv == 5) && !southRhombus)
      {
         if(polarPentagon)
         {
            Pointd t, i2, t2;
            int a = Min(r, nCapRows), b = r - a;

            move5x6Vertex(t, firstCentroid, nCapRows * u, 0);
            cross5x6Interruption(t, i2, false, false);
            move5x6Vertex(t2, i2, a * u, (a/2 + nCapRows) * u);
            if(b)
            {
               cross5x6Interruption(t2, i2, false, false);
               move5x6Vertex(rc, i2, (b/2) * u, b * u);
            }
            else
               rc = t2;
         }
         else
            move5x6Vertex(rc, firstCentroid,
                nCapRows * -u + ((r+1) >> 1) * u,
                nCapRows *  u + r * u);
      }
      else
         move5x6Vertex(rc, firstCentroid,
             nCapRows * -2 * u + (r >> 1) * -u,
             nCapRows * -1 * u + (r >> 1) *  u + (r & 1) * u);

      // Iterating through scanline
      for(col = 0; keepGoing && col < nCols - colSkip; col++)
      {
         if(index != -1)
         {
            col = (int)(index - i);
            i = index;
         }

         if(colSkip && southPentagon)
         {
            int jumpCol = (nCols - colSkip) >> 1;

            if(col <= jumpCol)
            {
               if(polarPentagon)
                  move5x6Vertex(centroid, rc, -col * u, 0);
               else
                  move5x6Vertex(centroid, rc, col * u, col * u);
            }
            else
            {
               Pointd t; // Avoid interruption

               if(polarPentagon)
               {
                  Pointd i2;
                  move5x6Vertex(t, rc, -jumpCol * u, 0);
                  cross5x6Interruption(t, i2, true, true);
                  move5x6Vertex(centroid, i2, 0, -(col - jumpCol) * u);
               }
               else
               {
                  int startShift = colSkip + jumpCol + 1;
                  int extraCols = col - jumpCol - 1;
                  move5x6Vertex(t, rc, startShift * u, startShift * u);
                  move5x6Vertex(centroid, t, extraCols * u, extraCols * u);
               }

               // REVIEW: This might have worked, but while move5x6Vertex() seems to be fine with it for the cap it is not here...
               // move5x6Vertex(centroid, rc, (jumpCol + extraCols + 1) * u, (jumpCol + extraCols + 1) * u);
            }
         }
         else if((edgeHex || nv == 5) && !southRhombus)
         {
            int n = polarPentagon ? nCapRows + r / 2 : nCols - 1 - nCapRows - r/2;
            int a = Min(col, n), b = col - a;
            Pointd t;

            // REVIEW: Issues with move5x6Vertex3() followed by cross5x6Interruption()
            if(!b || (nv == 5 && r > nMidRows/2))
            {
               if(polarPentagon && r <= nCapRows)
                  move5x6Vertex3(t, rc, 0, -a * u);
               else
                  move5x6Vertex3(t, rc, a * u, 0);
            }
            else
            {
               if(polarPentagon && r <= nCapRows)
                  move5x6Vertex(t, rc, 0, -a * u);
               else
                  move5x6Vertex(t, rc, a * u, 0);
            }

            if(b)
            {
               if(nv == 5 && r > nMidRows/2)
                  move5x6Vertex(centroid, t, polarPentagon ? 0 : b * u, b * u);
               else
               {
                  Pointd i2;
                  cross5x6Interruption(t, i2, false, polarPentagon);
                  if(polarPentagon)
                  {
                     int nb = nCapRows - r, c = col - (a + (b = Min(b, nb)));
                     Pointd t2;

                     move5x6Vertex(t2, i2, -b * u, -b * u);
                     if(c)
                     {
                        cross5x6Interruption(t2, i2, false, true);
                        move5x6Vertex3(centroid, i2, -c * u, 0);
                     }
                     else
                        centroid = t2;
                  }
                  else
                     move5x6Vertex(centroid, i2, b * u, b * u);
               }
            }
            else
               centroid = t;
         }
         else if((edgeHex || nv == 5) && southRhombus)
         {
            int n = polarPentagon ? nCapRows + r / 2 : nCols - 1 - nCapRows - r/2;
            int a = Min(col, n), b = col - a;
            Pointd t;

            if(b)
            {
               if(nv == 5 && r <= nMidRows/2 && !polarPentagon)
               {
                  move5x6Vertex3(t, rc, polarPentagon && r <= nCapRows ? 0 : a * u, a * u);
                  move5x6Vertex(centroid, t, b * u, b * u);
               }
               else
               {
                  Pointd i2;
                  // REVIEW: move5x6Vertex3() is required here
                  move5x6Vertex3(t, rc, polarPentagon && r <= nCapRows ? 0 : a * u, a * u);
                  cross5x6Interruption(t, i2, true, polarPentagon && r >= nMidRows/2);

                  if(polarPentagon)
                  {
                     int nb = nCapRows - r, c = col - (a + (b = Min(b, nb)));
                     Pointd t2;

                     move5x6Vertex(t2, i2, b * u, b * u);
                     if(c)
                     {
                        cross5x6Interruption(t2, i2, true, r >= nMidRows/2);
                        if(r >= nMidRows/2)
                           move5x6Vertex3(centroid, i2, 0, -c * u);
                        else
                           move5x6Vertex3(centroid, i2, c * u, 0);
                     }
                     else
                        centroid = t2;
                  }
                  else
                     move5x6Vertex(centroid, i2, b * u, nv == 5 ? b * u : 0);
               }
            }
            else
               move5x6Vertex3(centroid, rc, polarPentagon && r <= nCapRows ? 0 : a * u, a * u);
         }
         else
            move5x6Vertex(centroid, rc, col * u, col * u);

         if((keepGoing = centroidCallback(context, i, centroid))) i++;
      }
   }

   // Second cap
   for(r = 0, nCols -= 2; keepGoing && r < nCapRows - endCapSkip; r++, nCols -= 3)
   {
      int n = (edgeHex || nv == 5) && r < nCapRows / 2 ? nCapRows - (2*(r+1)) : 0;

      if(index != -1 && i + nCols <= index);

      // Compute start of scanline
      else if(polarPentagon)
      {
         if(southRhombus)
         {
            Pointd i2 { 2 + nCapRows*u, 4 - nCapRows*u };
            move5x6Vertex(rc, i2, -(r + 1) * 2 * u, -(r + 1) * u);
         }
         else
         {
            Pointd i2 { 3 - nCapRows*u, 2 + nCapRows*u };
            move5x6Vertex(rc, i2, (r + 1) * 2 * u, (r + 1) * u);
         }
      }
      else if((edgeHex || nv == 5) && !southRhombus)
      {
         Pointd t, i2;
         if(r < nCapRows / 2)
         {
            int a = Min(r + 1, nCapRows / 2), b = r + 1 - a;

            move5x6Vertex(t, firstCentroid,
                nCapRows * -u + (nMidRows >> 1) * u + a * 2 * u,
                nCapRows *  u + (nMidRows-1)    * u + a * u);
            if(b)
            {
               cross5x6Interruption(t, i2, false, false);
               move5x6Vertex(rc, i2, b * 2 * u, b * u);
            }
            else
               rc = t;
         }
         else
         {
            int a = nCapRows, ay = 3 * nCapRows - 1 - r;
            int b = 1 + 2 * (r - nCapRows / 2);

            move5x6Vertex(t, firstCentroid, a * u, a * u);
            cross5x6Interruption(t, i2, false, false);
            move5x6Vertex3(rc, i2, -ay * u, b * u);
         }
      }
      else if((edgeHex || nv == 5) && southRhombus && r >= nCapRows / 2)
      {
         Pointd t, i2;
         int a = nCapRows, ay = 2 * nCapRows + r + 1;
         int b = 1 + 2 * (r - nCapRows / 2);

         move5x6Vertex(t, firstCentroid, 0, a * u);
         cross5x6Interruption(t, i2, true, false);
         move5x6Vertex3(rc, i2, b * u, ay * u);
      }
      else
         move5x6Vertex3(rc, firstCentroid,
            nCapRows * -2 * u + ((nMidRows-1) >> 1) * -u + (r + 1) *     u,
            nCapRows * -1 * u + ((nMidRows-1) >> 1) *  u + (r + 1) * 2 * u);

      // Iterate through scanline
      for(col = 0; keepGoing && col < nCols; col++)
      {
         if(index == -1 || i == index)
         {
            if((edgeHex || nv == 5) && !southRhombus)
            {
               int a = Min(col, n), b = col - a;

               if(a)
               {
                  Pointd t;

                  if(b)
                  {
                     Pointd i2;
                     if(nv == 5)
                     {
                        b -= (nCols/2 - n/2) + r + r / 2 + 1;
                        if(polarPentagon)
                        {
                           move5x6Vertex(t, rc, a * u, 0);
                           cross5x6Interruption(t, i2, false, false);
                        }
                        else
                           move5x6Vertex3(i2, rc, a * u, 0);
                     }
                     else
                     {
                        move5x6Vertex(t, rc, a * u, 0);
                        cross5x6Interruption(t, i2, false, false);
                     }
                     if(polarPentagon)
                        move5x6Vertex3(centroid, i2, 0, b * u);
                     else
                        move5x6Vertex3(centroid, i2, b * u, b * u);
                  }
                  else
                     move5x6Vertex3(centroid, rc, a * u, 0);
               }
               else
                  move5x6Vertex3(centroid, rc, b * u, b * u);
            }
            else if((edgeHex || nv == 5) && southRhombus)
            {
               int a = Min(col, n), b = col - a;

               if(a)
               {
                  Pointd t;
                  if(polarPentagon) // REVIEW: move5x6Vertex3() breaks things here
                     move5x6Vertex(t, rc, -a * u, 0);
                  else
                     // REVIEW: Avoid using move5x6Vertex3() before cross5x6Interruption()
                     (b ? move5x6Vertex : move5x6Vertex3)(t, rc, a * u, a * u);
                  if(b)
                  {
                     Pointd i2;
                     cross5x6Interruption(t, i2, true, polarPentagon);
                     if(nv == 5)
                     {
                        b -= (nCols/2 - n/2) + r + r / 2 + 1;
                        if(polarPentagon)
                           move5x6Vertex3(centroid, i2, 0, -b * u);
                        else
                           move5x6Vertex3(centroid, i2, b * u, b * u);
                     }
                     else
                        move5x6Vertex3(centroid, i2, b * u, 0);
                  }
                  else
                     centroid = t;
               }
               else
                  move5x6Vertex3(centroid, rc, b * u, 0);
            }
            else
               move5x6Vertex(centroid, rc, col * u, col * u);

            if((keepGoing = centroidCallback(context, i, centroid))) i++;
         }
         else if(nv == 5 && n > col && n - col < index - i)
         {
            i += (int64)n - col + 1;
            col = n;
         }
         else
         {
            int ff = (int)Min(index - i, nCols - col);
            col += ff-1;
            i += ff;
         }

         if(nv == 5 && col == n)
            col = nCols - n - 1;
      }
   }

#ifdef _DEBUG
   if(keepGoing && i != ((Array)context).count)
   {
      PrintLn("WARNING: Mismatched sub-zone index");
   }
#endif
   return keepGoing ? -1 : i;
}

// This function handles variations #4 (basic), #11 (north edge hex), #12 (south edge hex), #19 (north pentagon), #20 (south pentagon), #27 (north pole) and #28 (south pole)
static int64 generateOddParentEvenDepth(void * context, bool (* centroidCallback)(void * context, uint64 index, Pointd centroid),
   Pointd firstCentroid, int rDepth, double u,
   int nv, bool polarPentagon, bool southRhombus, bool edgeHex, int64 index)
{
   // Start from hexagon / pentagon vertex

   // Bottom-To-Top Scanlines ("rows" are the vertical scanlines)
   // e.g., A6-0-E (level 1) with level 7 sub-zones (relative depth: 6)          -- first zone: D6-9-D
   // e.g., A6-0-D (level 1 north pentagon) with level 7 sub-zones (relative depth: 6) -- first zone: D4-1F8-D
   // e.g., A5-0-D (level 1 south pentagon) with level 7 sub-zones (relative depth: 6) -- first zone: D3-1F8-D
   /* rd: 2                   rd: 4             nRows = 13   maxCols = 10
                          10 10 10 10
                        7   9  9  9   7
      4   4           4                 4
        3
    1       1       1                     1

      x   x      2        x   x   x   x        4
        x      1            x   x   x        3
      x   x      2        x   x   x   x        4
    x   x   x      3    x   x   x   x   x        5
      x   x      2        x   x   x   x        4
        x      1        x   x   x   x   x        5
      x   x      2    x   x   x   x   x   x        6
                        x   x   x   x   x        5
                      x   x   x   x   x   x        6
    nRows = 5       x   x   x   x   x   x   x        7
    maxCols = 4       x   x   x   x   x   x        6
                        x   x   x   x   x        5
                      x   x   x   x   x   x        6
                        x   x   x   x   x        5
                          x   x   x   x        4
                        x   x   x   x   x        5
                          x   x   x   x        4
                            x   x   x        3
                          x   x   x   x        4

     rd: 6  (757 zones)
               28    28    28    28    28    28    28    28    28    28
             ..   27    27    27    27    27    27    27    27    27   ..
           10                                                            10
         7                                                                  7
       4                                                                      4
     1                                                                          1

   */
   bool keepGoing = true;
   Pointd centroid;
   bool northPentagon = nv == 5 && !southRhombus;
   bool southPentagon = nv == 5 && southRhombus;
   int nCapRows = (int)POW3((rDepth-2)/2), nMidRows = 2 * nCapRows + 1;
   int endCapSkip = nv == 5 ? (nCapRows + 1) / 2 : 0;
   int minCols = 1;
   int r, nCols, col;
   Pointd rc;
   int64 i = 0;

   // First cap
   for(r = 0, nCols = minCols; keepGoing && r < nCapRows; r++, nCols += 3)
   {
      if(index != -1 && i + nCols <= index)
      {
         i += nCols;
         continue;
      }

      // Compute start of scanline
      if(polarPentagon)
         move5x6Vertex(rc, firstCentroid, (southPentagon?-1:1) * r * u, (southPentagon?-1:1) * r * u);
      else
         move5x6Vertex(rc, firstCentroid, 0, r * u);

      // Iterate through scanline
      for(col = 0; keepGoing && col < nCols; col++)
      {
         if(index != -1)
         {
            col = (int)(index - i);
            i = index;
         }

         if(polarPentagon)
         {
            int n = r/2 + r;
            int a = Min(col, n), b = col - a;
            Pointd t;

            move5x6Vertex(t, rc, (southPentagon?-1:1) * -a * u/3, (southPentagon?-1:1) * -2*a * u/3);
            if(b)
            {
               Pointd i2;
               if(r & 1)
               {
                  b--;
                  move5x6Vertex(i2, t, (southPentagon?-1:1) * -u/3, (southPentagon?-1:1) * -u/3);
                  t = i2;
               }
               cross5x6Interruption(t, i2, southPentagon, !southPentagon);
               move5x6Vertex(centroid, i2, (southPentagon?-1:1) * (-b * 2 - (r&1)) *u/3, (southPentagon?-1:1) * (-b - (r&1)) * u/3);
            }
            else
               centroid = t;
         }
         else
            move5x6Vertex(centroid, rc, col * u/3, -col * u/3);
         if((keepGoing = centroidCallback(context, i, centroid))) i++;
      }
   }

   // Main section
   for(r = 0; keepGoing && r < nMidRows; r++, nCols += (nCols & 1) ? 1 : -1)
   {
      int skip = nv == 5 && r > nCapRows ? r - nCapRows : 0;

      if(index != -1 && i + nCols - skip <= index)
      {
         i += nCols - skip;
         continue;
      }

      // Compute start of scanline
      if(polarPentagon)
      {
         Pointd t, i2;

         move5x6Vertex(t, firstCentroid, (southPentagon?-1:1) * nCapRows * u, (southPentagon?-1:1) * nCapRows * u);
         cross5x6Interruption(t, i2, southPentagon, southPentagon);
         if(r)
         {
            int a = r >> 1, b = r & 1;
            move5x6Vertex(rc, i2,
                           (southPentagon?-1:1) * a * u + (southPentagon?-1:1) * b * 2 * u / 3,
                           (southPentagon?-1:1) * a * u + (southPentagon?-1:1) * b *     u / 3);
         }
         else
            rc = i2;
      }
      else if(southPentagon && r > nCapRows)
      {
         int a = r / 2, b = r & 1;
         Pointd t;

         move5x6Vertex(t, firstCentroid, nCapRows * u, nCapRows * u); // This is the pentagon centroid
         move5x6Vertex(rc, t, a * u + b * u / 3, nCapRows * u - b * u/3);
      }
      else if((edgeHex || southPentagon) && southRhombus && r)
      {
         int n = r + r/2;
         Pointd t, i2;

         move5x6Vertex(t, firstCentroid,
            (nCapRows + r) * 2 * u / 3 - (2*nCapRows - r) * u / 3,
            (nCapRows + r) *     u / 3 + (2*nCapRows - r) * u / 3);
         if(nv == 5 && r > nCapRows)
            i2 = t;
         else
            cross5x6Interruption(t, i2, true, false);
         move5x6Vertex(rc, i2, n * u / 3, n * 2 * u / 3);
      }
      else
         move5x6Vertex(rc, firstCentroid,
                        (r >> 1) * u + (r & 1) * 2 * u / 3,
            (nCapRows + (r >> 1)) * u + (r & 1) * u / 3);

      // Iterate through scanline
      for(col = 0; keepGoing && col < nCols - skip; col++)
      {
         if(index != -1)
         {
            col = (int)(index - i);
            i = index;
         }

         if(polarPentagon)
         {
            int r2 = r > nCapRows ? (2*nCapRows - r) : r;
            int n = r2 + r2/2;
            int a = Min(col, n), b = col - a;
            Pointd t;

            if(a)
               move5x6Vertex(t, rc, (southPentagon?-1:1) * a * u/3, (southPentagon?-1:1) * -a * u/3);
            else
               t = rc;
            if(b)
            {
               int maxB = r < nCapRows ? (nCapRows - r + (nCapRows - r)/2) : r-nCapRows /*(nCapRows - r2 + (nCapRows - r2)/2)*/, c = 0;
               Pointd i2;
               bool crossingLeft = southPentagon ? r >= nCapRows : r < nCapRows;

               if(b > maxB)
                  c = b - maxB, b = maxB;

               cross5x6Interruption(t, i2, southPentagon, crossingLeft);
               if(r >= nCapRows)
                  move5x6Vertex(centroid, i2, (southPentagon?-1:1) * b * 2*u/3, (southPentagon?-1:1) * b * u/3);
               else
                  move5x6Vertex(centroid, i2, (southPentagon?-1:1) * -b * u/3, (southPentagon?-1:1) * -b * 2*u/3);

               if(r < nCapRows)
               {
                  if(c)
                  {
                     int oddR = r&1;

                     if(!oddR)
                     {
                        c--;
                        move5x6Vertex(i2, centroid, (southPentagon?-1:1) * -u/3, (southPentagon?-1:1) * -u/3);
                        centroid = i2;
                     }
                     cross5x6Interruption(centroid, i2, southPentagon, northPentagon);

                     if(c > maxB)
                     {
                        a = Min(c, maxB);
                        b = c - a;
                        move5x6Vertex(t, i2, (southPentagon?-1:1) * (-a * 2 - (1-oddR)) * u/3, (southPentagon?-1:1) * (-a - (1-oddR)) * u/3);
                        cross5x6Interruption(t, i2, southPentagon, northPentagon);

                        move5x6Vertex(centroid, i2, (southPentagon?-1:1) * -b * u/3, (southPentagon?-1:1) * b * u/3);
                     }
                     else
                        move5x6Vertex(centroid, i2, (southPentagon?-1:1) * (-c * 2 - (1-oddR)) *u/3, (southPentagon?-1:1) * (-c - (1-oddR)) * u/3);
                  }
               }
               else
               {
                  i2 = centroid;
                  if(c > maxB)
                  {
                     a = Min(c, maxB);
                     b = c - a;
                     move5x6Vertex(t, i2, (southPentagon?-1:1) * a * u/3, (southPentagon?-1:1) * a * 2*u/3);
                     cross5x6Interruption(t, i2, southPentagon, crossingLeft);

                     move5x6Vertex(centroid, i2, (southPentagon?-1:1) * -b * u/3, (southPentagon?-1:1) * b * u/3);
                  }
                  else
                     move5x6Vertex(centroid, i2, (southPentagon?-1:1) * c *u/3, (southPentagon?-1:1) * c * 2*u/3);
               }
            }
            else
               centroid = t;
         }
         else if(northPentagon || (edgeHex && !southRhombus))
         {
            int n = northPentagon && r > nCapRows ? nCapRows + nCapRows / 2 - (r - nCapRows) / 2 : 3*nCapRows - r - (r + 1) / 2;
            int a = Min(col, n), b = col - a;
            Pointd t;

            move5x6Vertex(t, rc, a * u/3, -a * u/3);
            if(b)
            {
               Pointd i2;
               if(northPentagon && r > nCapRows)
                  i2 = t;
               else
                  cross5x6Interruption(t, i2, false, false);
               move5x6Vertex(centroid, i2, b * 2*u/3, b * u/3);
            }
            else
               centroid = t;
         }
         else if((edgeHex || southPentagon) && southRhombus && r)
         {
            int n = southPentagon && r > nCapRows ? nCapRows + nCapRows / 2 - (r - nCapRows) / 2 : r + r / 2;
            int a = Min(col, n), b = col - a;
            Pointd t;

            move5x6Vertex(t, rc, -a * u/3, -a * 2*u/3);
            if(b)
            {
               Pointd i2;
               if(southPentagon && r >= nCapRows)
                  i2 = t;
               else
                  cross5x6Interruption(t, i2, true, true);
               move5x6Vertex(centroid, i2, b * u/3, -b * u/3);
            }
            else
               centroid = t;
         }
         else
            move5x6Vertex(centroid, rc, col * u/3, -col * u/3);

         if((keepGoing = centroidCallback(context, i, centroid))) i++;
      }
   }

   // Second cap
   for(r = 0, nCols -= 2; keepGoing && r < nCapRows - endCapSkip; r++, nCols -= 3)
   {
      int n = nv == 5 ? (r >= nCapRows / 2 ? 0 : nCapRows - (2*(r+1))) : -1;

      if(index != -1 && i + nCols <= index);

      // Compute start of scanline
      else if(polarPentagon)
      {
         Pointd t, i2;
         int r2 = nMidRows-1, a = r2 >> 1, b = r2 & 1;

         move5x6Vertex(t, firstCentroid, (southPentagon?-1:1) * nCapRows * u, (southPentagon?-1:1) * nCapRows * u);
         cross5x6Interruption(t, i2, southPentagon, southPentagon);
         move5x6Vertex(t, i2,
                        (southPentagon?-1:1) * a * u + (southPentagon?-1:1) * b * 2 * u / 3,
                        (southPentagon?-1:1) * a * u + (southPentagon?-1:1) * b *     u / 3);
         cross5x6Interruption(t, i2, southPentagon, southPentagon);
         move5x6Vertex(rc, i2, (southPentagon?-1:1) * (r + 1) * u, (southPentagon?-1:1) * (r + 1) * u);
      }
      else if(southPentagon)
      {
         Pointd t;

         move5x6Vertex(t, firstCentroid, nCapRows * u, nCapRows * u); // This is the pentagon centroid
         move5x6Vertex(rc, t, (nMidRows / 2) * u, (nCapRows - r - 1) * u);
      }
      else if(edgeHex && !southRhombus)
      {
         Pointd t, i2;
         move5x6Vertex(t, firstCentroid, 3 * nCapRows * u/3, 3 * nCapRows * 2*u/3);
         cross5x6Interruption(t, i2, false, false);
         move5x6Vertex(rc, i2, (r + 1) * u, (r + 1) * u);
      }
      else if(edgeHex && southRhombus)
      {
         Pointd t, i2;
         move5x6Vertex(t, firstCentroid, 3 * nCapRows * 2*u/3, 3 * nCapRows * u/3);
         cross5x6Interruption(t, i2, true, false);
         move5x6Vertex(rc, i2, 3 * nCapRows * u / 3, 3 * nCapRows * 2 * u / 3 - (r + 1) * u);
      }
      else
         move5x6Vertex(rc, firstCentroid,
            ((nMidRows+1) / 2 + r) * u,
            (nCapRows + ((nMidRows-1) >> 1)) * u);

      // Iterate through scanline
      for(col = 0; keepGoing && col < nCols; col++)
      {
         Pointd t;
         int a = 0, b = 0;

         if(nv == 5)
         {
            a = Min(col, n);
            b = col - a;
            if(b)
               b -= (nCols/2 - n/2) + r + r / 2 + 1;
         }

         if(index == -1 || index == i)
         {
            if(polarPentagon)
            {
               move5x6Vertex(t, rc, (southPentagon?-1:1) * a * 2*u/3, (southPentagon?-1:1) * a * u/3);
               move5x6Vertex(centroid, t, (southPentagon?-1:1) * b * u/3, (southPentagon?-1:1) * b * 2*u/3);
            }
            else if(northPentagon)
            {
               move5x6Vertex(t, rc, a * u/3, -a * u/3);
               move5x6Vertex(centroid, t, b * 2*u/3, b * u/3);
            }
            else if(southPentagon)
            {
               move5x6Vertex(t, rc, -a * u/3, -2*a * u/3);
               move5x6Vertex(centroid, t, b * u/3, -b * u/3);
            }
            else if(edgeHex && !southRhombus)
               move5x6Vertex(centroid, rc, col * 2*u/3, col * u/3);
            else if(edgeHex && southRhombus)
               move5x6Vertex(centroid, rc, -col * u/3, -2*col * u/3);
            else
               move5x6Vertex(centroid, rc, col * u/3, -col * u/3);

            if((keepGoing = centroidCallback(context, i, centroid))) i++;
         }
         else if(nv == 5 && n > col && n - col < index - i)
         {
            i += (int64)n - col + 1;
            col = n;
         }
         else
         {
            int ff = (int)Min(index - i, nCols - col);
            col += ff-1;
            i += ff;
         }

         if(nv == 5 && col == n)
            col = nCols - n - 1;
      }
   }

#ifdef _DEBUG
   if(keepGoing && i != ((Array)context).count)
   {
      PrintLn("WARNING: Mismatched sub-zone index");
   }
#endif
   return keepGoing ? -1 : i;
}

static inline bool addCentroid(Array<Pointd> centroids, uint64 index, Pointd centroid)
{
   centroids[(uint)index] = centroid;
   return true;
}

Array<Pointd> getI3HSubZoneCentroids(I3HZone zone, int rDepth)
{
   // Even refinement level: Left-To-Right Scanlines (top-to-bottom scanline order)
   // Odd refinement level: Bottom-To-Top Scanlines (left-to-right scanline order)
   // Even relative depth: start on a vertex
   // Odd relative depth: start on an edge
   uint64 nSubZones = zone.getSubZonesCount(rDepth);
   // Each centroid is 16 bytes and array memory allocation currently does not support more than 4G
   if(nSubZones < 1LL<< (32-4))
   {
      Array<Pointd> centroids { size = (uint)nSubZones };

      if(rDepth > 0)
         iterateI3HSubZones(zone, rDepth, centroids, addCentroid, -1);
      else
         centroids[0] = zone.centroid;
#if 0 //def _DEBUG
      if(i > centroids.count)
         PrintLn("WARNING: Writing past centroids array");
#endif
      // centroids.count = i;

      return centroids;
   }
   return null;
}

/*static */int64 iterateI3HSubZones(I3HZone zone, int rDepth, void * context,
   bool (* centroidCallback)(void * context, uint64 index, Pointd centroid), int64 index)
{
   int64 stopIndex;
   Pointd firstCentroid;
   int level = zone.level, levelI9R = zone.levelI9R;
   int nv = zone.nPoints, subHex = zone.subHex, rootRhombus = zone.rootRhombus;
   uint64 rhombusIX = zone.rhombusIX;
   bool oddDepth = rDepth & 1, oddParent = subHex > 2;
   int szLevel = level + rDepth;
   double u = 1.0 / POW3((szLevel / 2));
   bool southRhombus = rootRhombus & 1;
   bool polarPentagon = subHex == 1 || subHex == 2 || subHex == 6 || subHex == 7;
   int divs = (int)POW3(levelI9R);
   // Edge Hexagons are either -A or -D
   bool edgeHex = nv == 6 && rhombusIX && (subHex == 0 || subHex == 3) && (southRhombus ? ((rhombusIX % divs) == 0) : ((rhombusIX / divs) == 0));

   zone.getFirstSubZoneCentroid(rDepth, firstCentroid);

#if 0 //def _DEBUG
   {
      I3HZone cKey = I3HZone::fromCentroid(szLevel, firstCentroid);
      char zoneId[100];
      cKey.getZoneID(zoneId);
      PrintLn(zoneId);
   }
#endif

   if(oddParent)
   {
      // Odd Level parents -- e.g., A6-0-E (level 1)
      if(oddDepth)
         stopIndex = generateOddParentOddDepth  (context, centroidCallback, firstCentroid, rDepth, u, nv, polarPentagon, southRhombus, edgeHex, index);
      else
         stopIndex = generateOddParentEvenDepth (context, centroidCallback, firstCentroid, rDepth, u, nv, polarPentagon, southRhombus, edgeHex, index);
   }
   else
   {
      // Even Level parents
      if(oddDepth)
         stopIndex = generateEvenParentOddDepth (context, centroidCallback, firstCentroid, rDepth, u, nv, polarPentagon, southRhombus, edgeHex, index);
      else
         stopIndex = generateEvenParentEvenDepth(context, centroidCallback, firstCentroid, rDepth, u, nv, polarPentagon, southRhombus, edgeHex, index);
   }
   return stopIndex;
}
