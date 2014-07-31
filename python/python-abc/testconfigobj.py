#!/usr/bin/env python
try:
    import sys
    sys.path.append('/usr/lib/linuxmint/common')
    from configobj import ConfigObj
except Exception, detail:
    print detail
    pass
def read_configuration():
    global icon_busy
    global icon_up2date
    global icon_updates
    global icon_error
    global icon_unknown
    global icon_apply

    config = ConfigObj("/etc/linuxmint/mintUpdate.conf")
    prefs = {}

    #Read refresh config
    try:
        prefs["timer_minutes"] = int(config['refresh']['timer_minutes'])
        prefs["timer_hours"] = int(config['refresh']['timer_hours'])
        prefs["timer_days"] = int(config['refresh']['timer_days'])
    except:
        prefs["timer_minutes"] = 15
        prefs["timer_hours"] = 0
        prefs["timer_days"] = 0

    #Read update config
    try:
        prefs["delay"] = int(config['update']['delay'])
        prefs["ping_domain"] = config['update']['ping_domain']
        prefs["dist_upgrade"] = (config['update']['dist_upgrade'] == "True")
    except:
        prefs["delay"] = 30
        prefs["ping_domain"] = "google.com"
        prefs["dist_upgrade"] = True

    #Read icons config
    try:
        icon_busy = config['icons']['busy']
        icon_up2date = config['icons']['up2date']
        icon_updates = config['icons']['updates']
        icon_error = config['icons']['error']
        icon_unknown = config['icons']['unknown']
        icon_apply = config['icons']['apply']
    except:
        icon_busy = "/usr/lib/linuxmint/mintUpdate/icons/base.svg"
        icon_up2date = "/usr/lib/linuxmint/mintUpdate/icons/base-apply.svg"
        icon_updates = "/usr/lib/linuxmint/mintUpdate/icons/base-info.svg"
        icon_error = "/usr/lib/linuxmint/mintUpdate/icons/base-error2.svg"
        icon_unknown = "/usr/lib/linuxmint/mintUpdate/icons/base.svg"
        icon_apply = "/usr/lib/linuxmint/mintUpdate/icons/base-exec.svg"

    #Read levels config
    try:
        prefs["level1_visible"] = (config['levels']['level1_visible'] == "True")
        prefs["level2_visible"] = (config['levels']['level2_visible'] == "True")
        prefs["level3_visible"] = (config['levels']['level3_visible'] == "True")
        prefs["level4_visible"] = (config['levels']['level4_visible'] == "True")
        prefs["level5_visible"] = (config['levels']['level5_visible'] == "True")
        prefs["level1_safe"] = (config['levels']['level1_safe'] == "True")
        prefs["level2_safe"] = (config['levels']['level2_safe'] == "True")
        prefs["level3_safe"] = (config['levels']['level3_safe'] == "True")
        prefs["level4_safe"] = (config['levels']['level4_safe'] == "True")
        prefs["level5_safe"] = (config['levels']['level5_safe'] == "True")
    except:
        prefs["level1_visible"] = True
        prefs["level2_visible"] = True
        prefs["level3_visible"] = True
        prefs["level4_visible"] = False
        prefs["level5_visible"] = False
        prefs["level1_safe"] = True
        prefs["level2_safe"] = True
        prefs["level3_safe"] = True
        prefs["level4_safe"] = False
        prefs["level5_safe"] = False    

    #Read columns config
    try:
        prefs["level_column_visible"] = (config['visible_columns']['level'] == "True")
    except:
        prefs["level_column_visible"] = True
    try:
        prefs["package_column_visible"] = (config['visible_columns']['package'] == "True")
    except:
        prefs["package_column_visible"] = True
    try:
        prefs["old_version_column_visible"] = (config['visible_columns']['old_version'] == "True")
    except:
        prefs["old_version_column_visible"] = True
    try:
        prefs["new_version_column_visible"] = (config['visible_columns']['new_version'] == "True")
    except:
        prefs["new_version_column_visible"] = True
    try:
        prefs["size_column_visible"] = (config['visible_columns']['size'] == "True")
    except:
        prefs["size_column_visible"] = True

    #Read window dimensions
    try:
        prefs["dimensions_x"] = int(config['dimensions']['x'])
        prefs["dimensions_y"] = int(config['dimensions']['y'])
        prefs["dimensions_pane_position"] = int(config['dimensions']['pane_position'])
    except:
        prefs["dimensions_x"] = 790
        prefs["dimensions_y"] = 540
        prefs["dimensions_pane_position"] = 230

    #Read package blacklist
    try:
        prefs["blacklisted_packages"] = config['blacklisted_packages']
    except:
        prefs["blacklisted_packages"] = []

    return prefs

prefs = read_configuration()
print prefs
