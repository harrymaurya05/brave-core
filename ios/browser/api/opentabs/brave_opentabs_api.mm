/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/ios/browser/api/opentabs/brave_opentabs_api.h"
#import <WebKit/WebKit.h>

#include "base/bind.h"
#include "base/strings/sys_string_conversions.h"

#include "components/sync_sessions/open_tabs_ui_delegate.h"
#include "components/sync_sessions/session_sync_service.h"

#include "ios/chrome/browser/browser_state/chrome_browser_state.h"
#include "ios/chrome/browser/ui/recent_tabs/synced_sessions.h"

#include "ios/web/public/thread/web_thread.h"
#include "net/base/mac/url_conversions.h"
#include "url/gurl.h"

#include "brave/ios/browser/api/opentabs/brave_opentabs_observer.h"
#include "brave/ios/browser/api/opentabs/opentabs_session_listener_ios.h"

#include "ios/chrome/browser/application_context.h"
#include "ios/chrome/browser/browser_state/chrome_browser_state.h"
#include "ios/chrome/browser/browser_state/chrome_browser_state_manager.h"
#include "ios/chrome/browser/main/browser_list.h"
#include "ios/chrome/browser/main/browser_list_factory.h"
#import "ios/chrome/browser/tabs/synced_window_delegate_browser_agent.h"
#import "ios/chrome/browser/web_state_list/web_state_list.h"
#include "ios/chrome/browser/tabs/tab_helper_util.h"
#include "ios/chrome/browser/sync/ios_chrome_synced_tab_delegate.h"
#include "ios/chrome/browser/web_state_list/web_state_opener.h"
#import "ios/chrome/browser/web/sad_tab_tab_helper.h"
#import "ios/chrome/browser/sessions/ios_chrome_session_tab_helper.h"
#include "ios/chrome/browser/complex_tasks/ios_task_tab_helper.h"
#include "ios/chrome/browser/ntp/new_tab_page_tab_helper.h"
#include "ios/chrome/browser/open_in/open_in_tab_helper.h"
#include "ios/chrome/browser/web/session_state/web_session_state_tab_helper.h"
#import "ios/web/navigation/navigation_manager_impl.h"
#include "ios/web/web_state/web_state_impl.h"

#import "ios/chrome/browser/main/browser.h"
#import "ios/chrome/browser/main/browser_list.h"
#import "ios/chrome/browser/main/browser_list_factory.h"
#import "ios/chrome/browser/app_launcher/app_launcher_tab_helper.h"
#import "ios/chrome/browser/snapshots/snapshot_tab_helper.h"
#import "ios/web/web_state/ui/crw_web_view_navigation_proxy.h"

#import "ios/chrome/browser/web_state_list/tab_insertion_browser_agent.h"
#import "ios/chrome/browser/snapshots/snapshot_browser_agent.h"
#import "ios/chrome/browser/tabs/closing_web_state_observer_browser_agent.h"
#import "ios/chrome/browser/tabs/synced_window_delegate_browser_agent.h"
#import "ios/chrome/browser/device_sharing/device_sharing_browser_agent.h"
#import "ios/chrome/browser/app_launcher/app_launcher_browser_agent.h"
#import "ios/chrome/browser/tabs/tab_parenting_browser_agent.h"
#import "ios/chrome/browser/web/web_navigation_browser_agent.h"
#import "ios/chrome/browser/web/web_state_delegate_browser_agent.h"
#import "ios/chrome/browser/policy/policy_watcher_browser_agent.h"
#import "ios/chrome/browser/send_tab_to_self/send_tab_to_self_browser_agent.h"
#import "ios/chrome/browser/main/browser_impl.h"

#include "brave/ios/browser/api/opentabs/brave_tab.h"


#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

namespace brave {
namespace ios {
sync_pb::SyncEnums::DeviceType
    SyncDeviceTypeFromSyncEnumsDeviceType(SyncDeviceType deviceType) {
  switch (deviceType) {
    case SyncDeviceTypeUnset:
      return sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_UNSET;
    case SyncDeviceTypeWin:
      return sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_WIN;
    case SyncDeviceTypeMac:
      return sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_MAC;
    case SyncDeviceTypeLinux:
      return sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_LINUX;
    case SyncDeviceTypeCros:
      return sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_CROS;
    case SyncDeviceTypeOther:
      return sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_OTHER;
     case SyncDeviceTypePhone:
      return sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_PHONE;
    default:
      return sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_UNSET;
  }
}

SyncDeviceType SyncEnumsDeviceTypeFromSyncDeviceType(
    sync_pb::SyncEnums::DeviceType deviceType) {
  switch (deviceType) {
    case sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_UNSET:
      return SyncDeviceTypeUnset;
    case sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_WIN:
      return SyncDeviceTypeWin;
    case sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_MAC:
      return SyncDeviceTypeMac;
    case sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_LINUX:
      return SyncDeviceTypeLinux;
    case sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_CROS:
      return SyncDeviceTypeCros;
    case sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_OTHER:
      return SyncDeviceTypeOther;
    case sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_PHONE:
      return SyncDeviceTypePhone;
    default:
      return SyncDeviceTypeUnset;
  }
}
}  // namespace ios
}  // namespace brave

#pragma mark - IOSOpenDistantTab

@implementation IOSOpenDistantTab

- (instancetype)initWithURL:(NSURL*)url
                      title:(nullable NSString*)title
                      tabId:(NSInteger)tabId
                 sessionTag:(NSString*)sessionTag {
  if ((self = [super init])) {
    self.url = url;
    self.title = title;
    self.tabId = tabId;
    self.sessionTag = sessionTag;
  }

  return self;
}

- (id)copyWithZone:(NSZone*)zone {
  IOSOpenDistantTab* openDistantTabCopy = [[[self class] allocWithZone:zone] init];

  if (openDistantTabCopy) {
    openDistantTabCopy.url = self.url;
    openDistantTabCopy.title = self.title;
    openDistantTabCopy.tabId = self.tabId;
    openDistantTabCopy.sessionTag = self.sessionTag;
  }

  return openDistantTabCopy;
}

- (void)updateOpenDistantTab:(NSURL*)url
                       title:(NSString*)title {
  [self setUrl:url];

  if ([title length] != 0) {
    [self setTitle:title];
  }
}

@end

#pragma mark - IOSOpenDistantSession

@implementation IOSOpenDistantSession

- (instancetype)initWithName:(nullable NSString*)name
                  sessionTag:(NSString*)sessionTag
                 dateCreated:(nullable NSDate*)modifiedTime
                  deviceType:(SyncDeviceType)deviceType
                        tabs:(NSArray<IOSOpenDistantTab*>*)tabs {
  if ((self = [super init])) {
    self.name = name;
    self.sessionTag = sessionTag;
    self.modifiedTime = modifiedTime;
    self.deviceType = deviceType;
    self.tabs = tabs;
  }

  return self;
}

- (id)copyWithZone:(NSZone*)zone {
  IOSOpenDistantSession* openDistantSession = [[[self class] allocWithZone:zone] init];

  if (openDistantSession) {
    openDistantSession.name = self.name;
    openDistantSession.sessionTag = self.sessionTag;
    openDistantSession.modifiedTime = self.modifiedTime;
    openDistantSession.deviceType = self.deviceType;
    openDistantSession.tabs = self.tabs;
  }

  return openDistantSession;
}

- (void)updateOpenDistantSessionModified:(NSDate*)modifiedTime {
    [self setModifiedTime:modifiedTime];
}

@end

#pragma mark - BraveOpenTabsAPI

namespace web {


}  // namespace web


@interface BraveOpenTabsAPI () {
  // Session Sync Service is needed in order to receive session details from different instances
  sync_sessions::SessionSyncService* sync_service_;
}
@end

@implementation BraveOpenTabsAPI

- (instancetype)initWithSessionSyncService:(sync_sessions::SessionSyncService*)sessionSyncService {
  if ((self = [super init])) {
    sync_service_ = sessionSyncService;
  }
  return self;
}

- (void)dealloc {
  sync_service_ = nullptr;
}

- (id<OpenTabsSessionServiceListener>)addObserver:(id<OpenTabsSessionServiceObserver>)observer {
  return [[OpenTabsSessionListenerImpl alloc] init:observer
                                       syncService:sync_service_];
}

- (void)removeObserver:(id<OpenTabsSessionServiceListener>)observer {
  [observer destroy];
}

- (void)getSyncedSessions:(void (^)(NSArray<IOSOpenDistantSession*>*))completion {
  // Getting SyncedSessions from SessionSyncService
  auto syncedSessions =
      std::make_unique<synced_sessions::SyncedSessions>(sync_service_);

  NSMutableArray<IOSOpenDistantSession*>* distantSessionList = [[NSMutableArray alloc] init];

  for (size_t sessionIndex = 0; sessionIndex < syncedSessions->GetSessionCount(); sessionIndex++) {
    const synced_sessions::DistantSession* session =
        syncedSessions->GetSession(sessionIndex);

    NSArray<IOSOpenDistantTab*>* distantTabs = [self onDistantTabResults:session->tabs];

    IOSOpenDistantSession* distantSession = [[IOSOpenDistantSession alloc] 
        initWithName:base::SysUTF8ToNSString(session->name)
          sessionTag:base::SysUTF8ToNSString(session->tag)
         dateCreated:session->modified_time.ToNSDate()
          deviceType:brave::ios::SyncEnumsDeviceTypeFromSyncDeviceType(session->device_type)
                tabs:distantTabs];
    [distantSessionList addObject: distantSession];
  }

  completion([distantSessionList copy]);
}

- (NSArray<IOSOpenDistantTab*>*)onDistantTabResults:
    (const std::vector<std::unique_ptr<synced_sessions::DistantTab>> &)distantTabList {
  NSMutableArray<IOSOpenDistantTab*>* distantTabs = [[NSMutableArray alloc] init];

  for (const auto& tab : distantTabList) {
    IOSOpenDistantTab* distantTab = [[IOSOpenDistantTab alloc] 
        initWithURL:net::NSURLWithGURL(tab->virtual_url)
              title:base::SysUTF16ToNSString(tab->title)
              tabId:tab->tab_id.id()
         sessionTag:base::SysUTF8ToNSString(tab->session_tag)];

    [distantTabs addObject: distantTab];
  }

  return [distantTabs copy];
}

@end
