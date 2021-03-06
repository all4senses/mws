{*
 +--------------------------------------------------------------------+
 | CiviCRM version 4.1                                                |
 +--------------------------------------------------------------------+
 | Copyright (C) 2011 Marty Wright                                    |
 | Licensed to CiviCRM under the Academic Free License version 3.0.   |
 +--------------------------------------------------------------------+
 | This file is a part of CiviCRM.                                    |
 |                                                                    |
 | CiviCRM is free software; you can copy, modify, and distribute it  |
 | under the terms of the GNU Affero General Public License           |
 | Version 3, 19 November 2007 and the CiviCRM Licensing Exception.   |
 |                                                                    |
 | CiviCRM is distributed in the hope that it will be useful, but     |
 | WITHOUT ANY WARRANTY; without even the implied warranty of         |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.               |
 | See the GNU Affero General Public License for more details.        |
 |                                                                    |
 | You should have received a copy of the GNU Affero General Public   |
 | License and the CiviCRM Licensing Exception along                  |
 | with this program; if not, contact CiviCRM LLC                     |
 | at info[AT]civicrm[DOT]org. If you have questions about the        |
 | GNU Affero General Public License or the licensing of CiviCRM,     |
 | see the CiviCRM license FAQ at http://civicrm.org/licensing        |
 +--------------------------------------------------------------------+
*}
{* this template is for configuring Scheduled Reminders *}

{if $action eq 1 or $action eq 2 or $action eq 8 or $action eq 16384}
   {include file="CRM/Admin/Form/ScheduleReminders.tpl"}
{else}
{if $rows}
    <div id="reminder">
      {include file="CRM/Admin/Page/Reminders.tpl"}
      <div class="action-link">
    	  <a href="{crmURL q="action=add&reset=1"}" id="newScheduleReminder" class="button"><span><div class="icon add-icon"></div>{ts}Add Reminder{/ts}</span></a>
      </div>

    </div>

{else}
    <div class="messages status">
      <div class="icon inform-icon"></div>
        {capture assign=crmURL}{crmURL p='civicrm/admin/scheduleReminders' q="action=add&reset=1"}{/capture}
        {ts 1=$crmURL}There are no Scheduled Reminders configured. You can <a href='%1'>add one</a>.{/ts}
    </div>    
{/if}
{/if}
