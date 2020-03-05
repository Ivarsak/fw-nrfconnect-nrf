#include <nrf9160.h>
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <stdio.h>
#include <disk/disk_access.h>
#include <fs/fs.h>
#include <logging/log.h>


LOG_MODULE_REGISTER(main);


u32_t T2B = 1900, B2T = 1100, StopPump = 1500;            // Pump Control
u32_t Valve_open = 40, Valve_closed = 100;                // valve Control
u32_t DelaY_UP = 10000, Delay_DOWN = 10000;               // based on the time the glider will take from surface to bottom and vice versa
u32_t DelaY_PUMP_T2B = 27000, DelaY_PUMP_B2T = 4000;     // defining pumping time 
u32_t SlowStart, SlowStop, dly = 10;                      // define smooth start and stop for motor

u32_t EscPin = DT_NORDIC_NRF_PWM_0_CH0_PIN, ValvePin = DT_NORDIC_NRF_PWM_0_CH0_PIN;
u32_t period = 20000;

double Temperature, Pressure;


static int lsdir(const char *path);



static FATFS fat_fs;

static struct fs_mount_t mp = {

	.type = FS_FATFS,

	.fs_data = &fat_fs,

};

static const char *disk_mount_pt = "/SD:";



void PWM(void)

{


 printk("PWM Application has started!\r\n");
       
        struct device *pwm_dev = device_get_binding(DT_NORDIC_NRF_PWM_0_LABEL);

        if (!pwm_dev) {
		printk("Cannot find %s!\n", "PWM0");
		return;
	}

 while(1)


    {
                  printk("while loop");
                   pwm_pin_set_usec(pwm_dev, DT_NORDIC_NRF_PWM_0_CH0_PIN, period, B2T);
                  
                   
                  k_sleep(2000);
           
                   pwm_pin_set_usec(pwm_dev, DT_NORDIC_NRF_PWM_0_CH0_PIN, period, T2B);
                     k_sleep(2000);

	
		}

		}


   void Get_pressure(void)

   {

   struct sensor_value oversampling_rate = { 8192, 0 };
	struct device *dev = device_get_binding(DT_INST_0_MEAS_MS5837_LABEL);

	if (dev == NULL) {
		LOG_ERR("Could not find MS5837 device, aborting test.");
		return;
	}

	if (sensor_attr_set(dev, SENSOR_CHAN_ALL, SENSOR_ATTR_OVERSAMPLING,
				&oversampling_rate) != 0) {
		LOG_ERR("Could not set oversampling rate of %d "
			"on MS5837 device, aborting test.",
			oversampling_rate.val1);
		return;
	}

	while (1) {
		struct sensor_value temp;
		struct sensor_value press;

		sensor_sample_fetch(dev);
		sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
		sensor_channel_get(dev, SENSOR_CHAN_PRESS, &press);

		//printf("Temperature: %d.%06d, Pressure: %d.%06d\n", temp.val1,
		  //     temp.val2, press.val1, press.val2);

                      Temperature = sensor_value_to_double(&temp);
                       Pressure = sensor_value_to_double(&press);

                      

                       printf("Temp: %f, Pressure: %f\n", Temperature, Pressure);

                       k_sleep(K_MSEC(1000));

		
	}

        }





void main(void)



{


/* raw disk i/o */

	do {

		static const char *disk_pdrv = "SD";

		u64_t memory_size_mb;

		u32_t block_count;

		u32_t block_size;



		if (disk_access_init(disk_pdrv) != 0) {

			LOG_ERR("Storage init ERROR!");

			break;

		}



		if (disk_access_ioctl(disk_pdrv,

				DISK_IOCTL_GET_SECTOR_COUNT, &block_count)) {

			LOG_ERR("Unable to get sector count");

			break;

		}

		LOG_INF("Block count %u", block_count);



		if (disk_access_ioctl(disk_pdrv,

				DISK_IOCTL_GET_SECTOR_SIZE, &block_size)) {

			LOG_ERR("Unable to get sector size");

			break;

		}

		printk("Sector size %u\n", block_size);



		memory_size_mb = (u64_t)block_count * block_size;

		printk("Memory Size(MB) %u\n", (u32_t)memory_size_mb>>20);

	} while (0);



	mp.mnt_point = disk_mount_pt;



	int res = fs_mount(&mp);



	if (res == FR_OK) {

		printk("Disk mounted.\n");

		lsdir(disk_mount_pt);

	} else {

		printk("Error mounting disk.\n");

	}



	while (1) {

		k_sleep(K_MSEC(1000));

	}

}



static int lsdir(const char *path)

{

	int res;

	struct fs_dir_t dirp;

	static struct fs_dirent entry;



	/* Verify fs_opendir() */

	res = fs_opendir(&dirp, path);

	if (res) {

		printk("Error opening dir %s [%d]\n", path, res);

		return res;

	}



	printk("\nListing dir %s ...\n", path);

	for (;;) {

		/* Verify fs_readdir() */

		res = fs_readdir(&dirp, &entry);



		/* entry.name[0] == 0 means end-of-dir */

		if (res || entry.name[0] == 0) {

			break;

		}



		if (entry.type == FS_DIR_ENTRY_DIR) {

			printk("[DIR ] %s\n", entry.name);

		} else {

			printk("[FILE] %s (size = %zu)\n",

				entry.name, entry.size);

		}

	}



	/* Verify fs_closedir() */

	fs_closedir(&dirp);



	return res;
         
   

	
        }

        
        
    


