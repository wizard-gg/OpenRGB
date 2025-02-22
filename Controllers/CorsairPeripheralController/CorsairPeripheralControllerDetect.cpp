#include "Detector.h"
#include "CorsairPeripheralController.h"
#include "CorsairK100Controller.h"
#include "CorsairK55RGBPROController.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_CorsairPeripheral.h"
#include "RGBController_CorsairK100.h"
#include "RGBController_CorsairK55RGBPRO.h"
#include <hidapi/hidapi.h>

#define CORSAIR_PERIPHERAL_CONTROLLER_NAME "Corsair peripheral"

/*-----------------------------------------------------*\
| Corsair vendor ID                                     |
\*-----------------------------------------------------*/
#define CORSAIR_VID                     0x1B1C

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
|   List taken from ckb-next                            |
|   Non-RGB keyboards were omitted from this list       |
\*-----------------------------------------------------*/
#define CORSAIR_K55_RGB_PID             0x1B3D

#define CORSAIR_K65_RGB_PID             0x1B17
#define CORSAIR_K65_LUX_RGB_PID         0x1B37
#define CORSAIR_K65_RGB_RAPIDFIRE_PID   0x1B39

#define CORSAIR_K68_RGB                 0x1B4F

#define CORSAIR_K70_RGB_PID             0x1B13
#define CORSAIR_K70_LUX_RGB_PID         0x1B33
#define CORSAIR_K70_RGB_RAPIDFIRE_PID   0x1B38
#define CORSAIR_K70_RGB_MK2_PID         0x1B49
#define CORSAIR_K70_RGB_MK2_SE_PID      0x1B6B
#define CORSAIR_K70_RGB_MK2_LP_PID      0x1B55

#define CORSAIR_K95_RGB_PID             0x1B11
#define CORSAIR_K95_PLATINUM_PID        0x1B2D

#define CORSAIR_STRAFE_PID              0x1B20
#define CORSAIR_STRAFE_RED_PID          0x1B44
#define CORSAIR_STRAFE_MK2_PID          0x1B48

/*-----------------------------------------------------*\
| Mouse product IDs                                     |
|   List taken from ckb-next                            |
\*-----------------------------------------------------*/
#define CORSAIR_GLAIVE_RGB_PID          0x1B34
#define CORSAIR_GLAIVE_RGB_PRO_PID      0x1B74
#define CORSAIR_HARPOON_RGB_PID         0x1B3C
#define CORSAIR_HARPOON_RGB_PRO_PID     0x1B75
#define CORSAIR_IRONCLAW_RGB_PID        0x1B5D
#define CORSAIR_M65_PID                 0x1B12
#define CORSAIR_M65_PRO_PID             0x1B2E
#define CORSAIR_M65_RGB_ELITE_PID       0x1B5A
#define CORSAIR_NIGHTSWORD_PID          0x1B5C
#define CORSAIR_SCIMITAR_PRO_RGB_PID    0x1B3E
#define CORSAIR_SABRE_RGB_PID           0x1B2F

/*-----------------------------------------------------*\
| Mousepad product IDs                                  |
|   List taken from ckb-next                            |
\*-----------------------------------------------------*/
#define CORSAIR_MM800_RGB_POLARIS_PID   0x1B3B

/*-----------------------------------------------------*\
| Headset Stand product IDs                             |
|   List taken from ckb-next                            |
\*-----------------------------------------------------*/
#define CORSAIR_ST100_PID               0x0A34

/*-----------------------------------------------------*\
| Corsair K100 Keyboard product ID                      |
| This keyboard uses a separate driver                  |
\*-----------------------------------------------------*/
#define CORSAIR_K100_PID                0x1B7C

void DetectCorsairK100Controllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CorsairK100Controller* controller = new CorsairK100Controller(dev, info->path);
        controller->SetName(name);

        if(controller->GetKeyboardType() != CORSAIR_TYPE_UNKNOWN)
        {
            RGBController_CorsairK100* rgb_controller = new RGBController_CorsairK100(controller);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            delete controller;
        }
    }
}   /* DetectCorsairPeripheralControllers() */

/*-----------------------------------------------------*\
| Corsair K55 RGB PRO Keyboard product ID               |
| This keyboard uses a separate driver                  |
\*-----------------------------------------------------*/
#define CORSAIR_K55_RGB_PRO_PID         0x1BA4

void DetectCorsairK55RGBPROControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        CorsairK55RGBPROController*     controller     = new CorsairK55RGBPROController(dev, info->path);
        controller->SetName(name);
        RGBController_CorsairK55RGBPRO* rgb_controller = new RGBController_CorsairK55RGBPRO(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}   /* DetectCorsairK55RGBPROControllers() */

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairPeripheralControllers                                                     *
*                                                                                          *
*       Tests the USB address to see if a Corsair RGB Keyboard controller exists there.    *
*                                                                                          *
\******************************************************************************************/
void DetectCorsairPeripheralControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        LOG_DEBUG("[%s] Device opened. VID/PID %02X:%02X", CORSAIR_PERIPHERAL_CONTROLLER_NAME, info->vendor_id , info->product_id);

        CorsairPeripheralController* controller = new CorsairPeripheralController(dev, info->path);
        controller->SetName(name);

        if(controller->GetDeviceType() != DEVICE_TYPE_UNKNOWN)
        {
            RGBController_CorsairPeripheral* rgb_controller = new RGBController_CorsairPeripheral(controller);
            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
        else
        {
            LOG_DEBUG("[%s] Device type is unknown", CORSAIR_PERIPHERAL_CONTROLLER_NAME);
            delete controller;
        }
    }
}   /* DetectCorsairPeripheralControllers() */

/*-----------------------------------------------------------------------------------------------------*\
| Keyboards                                                                                             |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Corsair K55 RGB",                 DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K55_RGB_PID,           1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K65 RGB",                 DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K65_RGB_PID,           1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K65 LUX RGB",             DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K65_LUX_RGB_PID,       1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K65 RGB RAPIDFIRE",       DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K65_RGB_RAPIDFIRE_PID, 1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K68 RGB",                 DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K68_RGB,               1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K70 RGB",                 DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K70_RGB_PID,           1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K70 LUX RGB",             DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K70_LUX_RGB_PID,       1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K70 RGB RAPIDFIRE",       DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K70_RGB_RAPIDFIRE_PID, 1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K70 RGB MK.2",            DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K70_RGB_MK2_PID,       1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K70 RGB MK.2 SE",         DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K70_RGB_MK2_SE_PID,    1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K70 RGB MK.2 Low Profile",DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K70_RGB_MK2_LP_PID,    1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K95 RGB",                 DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K95_RGB_PID,           1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair K95 RGB PLATINUM",        DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_K95_PLATINUM_PID,      1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Strafe",                  DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_STRAFE_PID,            1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Strafe Red",              DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_STRAFE_RED_PID,        1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Strafe MK.2",             DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_STRAFE_MK2_PID,        1, 0xFFC2);
/*-----------------------------------------------------------------------------------------------------*\
| Mice                                                                                                  |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Corsair Glaive RGB",              DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_GLAIVE_RGB_PID,        1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Glaive RGB PRO",          DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_GLAIVE_RGB_PRO_PID,    1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Harpoon RGB",             DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_HARPOON_RGB_PID,       1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Harpoon RGB PRO",         DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_HARPOON_RGB_PRO_PID,   1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Ironclaw RGB",            DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_IRONCLAW_RGB_PID,      1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair M65",                     DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_M65_PID,               1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair M65 PRO",                 DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_M65_PRO_PID,           1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair M65 RGB Elite",           DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_M65_RGB_ELITE_PID,     1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Nightsword",              DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_NIGHTSWORD_PID,        1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Scimitar PRO RGB",        DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_SCIMITAR_PRO_RGB_PID,  1, 0xFFC2);
REGISTER_HID_DETECTOR_IP("Corsair Sabre RGB",               DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_SABRE_RGB_PID,         1, 0xFFC2);

/*-----------------------------------------------------------------------------------------------------*\
| Mousemats                                                                                             |
\*-----------------------------------------------------------------------------------------------------*/
#ifdef USE_HID_USAGE
REGISTER_HID_DETECTOR_P("Corsair MM800 RGB Polaris",        DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_MM800_RGB_POLARIS_PID, 0xFFC2);
#else
REGISTER_HID_DETECTOR_I("Corsair MM800 RGB Polaris",        DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_MM800_RGB_POLARIS_PID, 0);
#endif
/*-----------------------------------------------------------------------------------------------------*\
| Headset Stands                                                                                        |
\*-----------------------------------------------------------------------------------------------------*/
#ifdef USE_HID_USAGE
REGISTER_HID_DETECTOR_P("Corsair ST100 RGB",                DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_ST100_PID,             0xFFC2);
#else
REGISTER_HID_DETECTOR_I("Corsair ST100 RGB",                DetectCorsairPeripheralControllers, CORSAIR_VID, CORSAIR_ST100_PID,             0);
#endif

/*-----------------------------------------------------------------------------------------------------*\
| Corsair K100 Keyboard                                                                                 |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Corsair K100",                    DetectCorsairK100Controllers,       CORSAIR_VID, CORSAIR_K100_PID,              1, 0xFF42);

/*-----------------------------------------------------------------------------------------------------*\
| Corsair K55 RGB PRO Keyboard                                                                          |
\*-----------------------------------------------------------------------------------------------------*/
REGISTER_HID_DETECTOR_IP("Corsair K55 RGB PRO",             DetectCorsairK55RGBPROControllers,  CORSAIR_VID, CORSAIR_K55_RGB_PRO_PID,       1, 0xFF42);
