/* based on code from:
http://jamesreuss.wordpress.com/2011/12/23/working-out-how-to-use-libfreenect-in-objective-c/
*/

#include <ros/ros.h>
#include <freenect/libfreenect.h>
#include <freenect/libfreenect-registration.h>
#include <stdio.h>

freenect_context *freenectContext;
freenect_device *freenectDevice;
int noDevicesConnected;
int error;

int main(int argc, char **argv) {

    ros::init(argc,argv,"test");
    ros::Time::init();

    int idx = 0;
    ros::param::get("~idx", idx);

    // freenect_init initialises a freenect context. The second parameter can be NULL if not using mutliple contexts.
    // freenect_set_log_level sets the log level for the specified freenect context.
    // freenect_select_subdevices selects which subdevices to open when connecting to a new kinect device.
    freenect_init(&freenectContext, NULL);
    freenect_set_log_level(freenectContext, FREENECT_LOG_DEBUG);
    freenect_select_subdevices(freenectContext, (freenect_device_flags)(FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));

    noDevicesConnected = freenect_num_devices(freenectContext);
    printf("Number of devices connected: %d\n", noDevicesConnected);
    // Exit the app if there are no devices connected.
    if (noDevicesConnected < 1) return 1;

    // freenect_open_device opens a Kinect device.
    error = freenect_open_device(freenectContext, &freenectDevice, idx);
    if (error < 0) {
        // Then exit the app if there was an error while connecting.
        printf("Could not open the Kinect device.\n");
        return 1;
    }

    freenect_registration registration = freenect_copy_registration(freenectDevice);
    printf("Baseline: %f\n", registration.zero_plane_info.dcmos_emitter_dist);
    printf("Focal length: %f\n", registration.zero_plane_info.reference_distance);
    printf("Focal length: %f\n", registration.zero_plane_info.reference_pixel_size);


    //freenect_set_tilt_degs(freenectDevice, 0);
    freenect_set_led(freenectDevice, LED_BLINK_RED_YELLOW);
    printf("Done Functions\n");

    freenect_close_device(freenectDevice);
    freenect_shutdown(freenectContext);
    printf("Done!\n");

    return 0;
}
