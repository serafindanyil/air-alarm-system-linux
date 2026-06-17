INTERNET_MONITOR_SERVICE_VERSION = 1.0
INTERNET_MONITOR_SERVICE_SITE = $(BR2_EXTERNAL_AIR_ALARM_SYSTEM_PATH)/internet-monitor-service
INTERNET_MONITOR_SERVICE_SITE_METHOD = local

INTERNET_MONITOR_SERVICE_DEPENDENCIES = \
	sdbus-cpp

define INTERNET_MONITOR_SERVICE_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/internet-monitor-service \
		$(TARGET_DIR)/usr/bin/internet-monitor-service

	$(INSTALL) -D -m 0644 \
		$(BR2_EXTERNAL_AIR_ALARM_SYSTEM_PATH)/package/internet-monitor-service/internet-monitor-service.service \
		$(TARGET_DIR)/usr/lib/systemd/system/internet-monitor-service.service

	$(INSTALL) -D -m 0644 \
		$(BR2_EXTERNAL_AIR_ALARM_SYSTEM_PATH)/package/internet-monitor-service/org.airalarm.InternetMonitor.conf \
		$(TARGET_DIR)/etc/dbus-1/system.d/org.airalarm.InternetMonitor.conf

	mkdir -p $(TARGET_DIR)/etc/systemd/system/multi-user.target.wants

	ln -sf /usr/lib/systemd/system/internet-monitor-service.service \
		$(TARGET_DIR)/etc/systemd/system/multi-user.target.wants/internet-monitor-service.service
endef

$(eval $(cmake-package))
