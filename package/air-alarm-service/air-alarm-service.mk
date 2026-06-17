AIR_ALARM_SERVICE_VERSION = 1.0
AIR_ALARM_SERVICE_SITE = $(BR2_EXTERNAL_AIR_ALARM_SYSTEM_PATH)/service
AIR_ALARM_SERVICE_SITE_METHOD = local

AIR_ALARM_SERVICE_DEPENDENCIES = \
	nlohmann-json \
	cpr \
	libgpiod2 \
	sdbus-cpp

AIR_ALARM_SERVICE_INSTALL_STAGING = NO

define AIR_ALARM_SERVICE_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/air-alarm-service \
		$(TARGET_DIR)/usr/bin/air-alarm-service

	$(INSTALL) -D -m 0755 \
		$(BR2_EXTERNAL_AIR_ALARM_SYSTEM_PATH)/package/air-alarm-service/air-alarm-config \
		$(TARGET_DIR)/usr/bin/air-alarm-config

	$(INSTALL) -D -m 0755 \
		$(BR2_EXTERNAL_AIR_ALARM_SYSTEM_PATH)/package/air-alarm-service/air-alarm-list \
		$(TARGET_DIR)/usr/bin/air-alarm-list

	$(INSTALL) -D -m 0644 \
		$(BR2_EXTERNAL_AIR_ALARM_SYSTEM_PATH)/service/config/air_alarm_config.json \
		$(TARGET_DIR)/etc/air-alarm-service/config.json

	$(INSTALL) -D -m 0644 \
		$(BR2_EXTERNAL_AIR_ALARM_SYSTEM_PATH)/service/config/regions_adjacency_list.json \
		$(TARGET_DIR)/etc/air-alarm-service/regions_adjacency_list.json

	$(INSTALL) -D -m 0644 \
		$(BR2_EXTERNAL_AIR_ALARM_SYSTEM_PATH)/package/air-alarm-service/air-alarm-service.service \
		$(TARGET_DIR)/usr/lib/systemd/system/air-alarm-service.service

	mkdir -p $(TARGET_DIR)/etc/systemd/system/multi-user.target.wants

	ln -sf /usr/lib/systemd/system/air-alarm-service.service \
		$(TARGET_DIR)/etc/systemd/system/multi-user.target.wants/air-alarm-service.service
endef

$(eval $(cmake-package))
