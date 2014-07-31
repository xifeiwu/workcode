#!/usr/bin/python
import os
import sys
import apt
import string
def main():
    package = sys.argv[1]
    level = 3 # Level 3 by default
    extraInfo = ""
    warning = ""
    rulesFile = open("/usr/lib/linuxmint/mintUpdate/rules","r")
    rules = rulesFile.readlines()
    goOn = True
    foundPackageRule = False # whether we found a rule with the exact package name or not
    for rule in rules:
        if (goOn == True):
            rule_fields = rule.split("|")
            if (len(rule_fields) == 5):
                rule_package = rule_fields[0]
                rule_version = rule_fields[1]
                rule_level = rule_fields[2]
                rule_extraInfo = rule_fields[3]
                rule_warning = rule_fields[4]
                if (rule_package == package):
                    foundPackageRule = True
                    print("rule_package:\t%s" % rule_package)
                    if (rule_version == newVersion):
                        print("rule_version\t%s" % rule_version)
                        level = rule_level
                        extraInfo = rule_extraInfo
                        warning = rule_warning
                        goOn = False # We found a rule with the exact package name and version, no need to look elsewhere
                    else:
                        if (rule_version == "*"):
                            level = rule_level
                            extraInfo = rule_extraInfo
                            warning = rule_warning
                else:
                    if (rule_package.startswith("*")):
                        keyword = rule_package.replace("*", "")
                        index = package.find(keyword)
                        if (index > -1 and foundPackageRule == False):
                            level = rule_level
                            extraInfo = rule_extraInfo
                            warning = rule_warning
    print("foundPackageRule:%s" % foundPackageRule)
    print("goOn:%s" % goOn)
    print("level:%s\textraInfo:%s\twarning:%s" % (level, extraInfo, warning))
    rulesFile.close()
    print("level:\t%s" % level)
if __name__ == "__main__":
    main()
