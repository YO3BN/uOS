// Apr 12, 2020


#ifndef __DRIVERS_H__
#define __DRIVERS_H__


/* Returned values for driver functions. */
//todo negative values + description
enum
{
  DRV_STATUS_ERROR = -1,
  DRV_STATUS_BUSY,
  DRV_STATUS_SUCCESS,
} DRV_STATUS_T;


/* Driver control commands, used for drv_ctrl_xxx functions. */

enum
{
  DRVCTRL_NONE = 0,
  DRVCTRL_GET,  /* Get driver parameters. See, specific driver headers. */
  DRVCTRL_SET,  /* Set driver parameters. See, specific driver headers. */
} DRVCTRL;

#endif /* __DRIVERS_H__ */
