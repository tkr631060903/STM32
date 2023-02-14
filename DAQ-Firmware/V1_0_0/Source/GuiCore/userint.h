#ifndef _DFDSJKLDSJLFSDJ_H_
#define _DFDSJKLDSJLFSDJ_H_

/*
    VioUI 使用方法：
    
    1. 使用CVI/Labwindows 2013在Source/GUI/GUIMaker目录下编辑uir文件。支持面板以及文本、文本框、按钮、图片按钮、组合框、图片这六种控件。
    2. 如果里面有图片，请使用Source/GUI/GUIMaker目录下的PictureTool.exe将图片保存在对应的ROM文件中。将该控件的Label Name设置为图片的起始地址。
    3. 保存.uir文件。Source/GUI/GUIMaker目录下会自动更新对应的.h文件。将该.h 文件加入工程中。
    4. 点击Labwindows/CVI菜单中的Options/Save in text format。保存为.tui 文件。
    5. 打开Source/GUI/GUIMaker目录下对应的.h文件，将所有的CVICALLBACK 关键词删掉。（只删除该单词）
    6. 运行Source/GUI目录下的tui文件转C代码.exe程序。选中生成的.tui文件，会生成两个文件：Gui_Content.c以及Gui_Content.h。将这两个文件加入工程中。
    7. 在对应的面板的C文件中编辑回调函数的内容。回调函数格式如下：
        int callback(int panel, int control, int event, void* callbackData,int eventData1, int eventData2)
    8. 大功告成。

    以下函数使用方法基本跟CVI对应函数雷同。
*/

void RunUserInterface();
void QuitUserInterface();
int DisplayPanel(u16 panelHandle);
u16 LoadPanel(u16 panelId);
int InstallPopup(u16 panelHandle);
void RemovePopup(u16 removePopup);
int SetCtrlAttribute(u16 panel, u16 control, int controlAttribute, ...);
int GetCtrlAttribute(u16 panel, u16 control, int controlAttribute, void* attributeValue);
int SetCtrlVal (u16 panelHandle, u16 controlID, ...);
int GetCtrlVal (u16 panelHandle, u16 controlID, void *value);
int RefreshCtrl(u16 panel, u16 control);
int RefreshPanel(u16 panel);
u16 GetActivePanel();
u16 GetBasePanel();
int DelayWithEventProcessing (int numberOfMilliSeconds);


/**********************************Color******************************************/
#define   BLUE0    0xEE56      //浅蓝色
#define   BLACK    0x0000      	//黑色
#define   BLUE     0xF800      	//蓝色
#define   RED      0x001F      	//红色
#define   GRAY0    0xCE9A   	//灰色0 3165 00110 001011 00101
#define   GRAY1    0x8410      	//灰色1      00000 000000 00000
#define   GRAY2    0x4208      	//灰色2  1111111111011111
#define   WHITE    0xFFFF	//白色   110010 110100 110100
#define   GREEN    0x07E0	//白色   110010 110100 110100
#define   YELLOW   0xF800      	//黄色07FF


/**********************************EVENT******************************************/
#define EVENT_NONE							0
#define EVENT_COMMIT						1   /* for menu selection events calling the main callback: */
													/* eventData1 = panel for menu selections */
													/* eventData2 = (int)(callbackData for menuItem) */
												/* for table events calling the control callback: */
													/* eventData1 = row of cell where event was generated; if 0, event affected multiple cells */
													/* eventData2 = column of cell where event was generated; if 0, event affected multiple cells */
												/* for tree events calling the control callback: */
													/* eventData1 = MARK_STATE_CHANGE, ... */
													/* eventData2 = depends on eventData1 */
#define EVENT_VAL_CHANGED					2   /* for table events calling the control callback: */
													/* eventData1 = row of cell where event was generated; if 0, event affected multiple cells  */
													/* eventData2 = column of cell where event was generated; if 0, event affected multiple cells  */
												/* for tree events calling the control callback: */
													/* eventData1 = ACTIVE_ITEM_CHANGE, ... */
													/* eventData2 = depends on eventData1 */
#define EVENT_LEFT_CLICK					3   /* eventData1 = mouse vertical pos., */
												/* eventData2 = mouse horizontal pos. */
#define EVENT_LEFT_DOUBLE_CLICK				4   /* eventData1 = mouse vertical pos., */
												/* eventData2 = mouse horizontal pos. */
#define EVENT_RIGHT_CLICK					5   /* eventData1 = mouse vertical pos., */
												/* eventData2 = mouse horizontal pos. */
#define EVENT_RIGHT_DOUBLE_CLICK			6   /* eventData1 = mouse vertical pos., */
												/* eventData2 = mouse horizontal pos. */
#define EVENT_KEYPRESS						7   /* eventData1 = keypressed, */
												/* eventData2 = call KeyPressEventIsLeadByte, KeyPressEventIsTrailByte, 
													GetKeyPressEventVirtualKey, GetKeyPressEventCharacter, 
													GetKeyPressEventModifiers, and SetKeyPressEventKey to obtain or set 
													the value of eventData2 */
#define EVENT_GOT_FOCUS						8   /* eventData1 = old control with focus */
#define EVENT_LOST_FOCUS					9   /* eventData1 = new control with focus */
#define EVENT_IDLE							10
#define EVENT_CLOSE							11
#define EVENT_PANEL_SIZE					12
#define EVENT_PANEL_MOVE					13
#define EVENT_END_TASK						14  /* sent to the MainCallback function when the system wants to quit */
												/* return  0L to quit,  1L to abort the termination */
#define EVENT_TIMER_TICK					15  /* eventData1 = pointer (double*) to current time */
												/* eventData2 = pointer (double*) to time elapsed since the control last
													received EVENT_TIMER_TICK */
												/* Instead of using values returned by eventData1 and eventData2, use 
													GetTimerTickData to retrieve current time value and elapsed time 
													values. For 32-bit only applications, you can use values returned
													by eventData1 and eventData2. */
#define EVENT_DISCARD						16  /* sent to panels and controls when they are discarded */
#define EVENT_EXPAND						17
#define EVENT_COLLAPSE						18
#define EVENT_DRAG							19
#define EVENT_DROP							20
#define EVENT_DROPPED						21
#define EVENT_SORT							22
#define EVENT_SELECTION_CHANGE				23
#define EVENT_HSCROLL						24
#define EVENT_VSCROLL						25
#define EVENT_MARK_STATE_CHANGE				26
#define EVENT_COMBO_BOX_INSERT				27  /* eventData1 = row of cell where event was generated */
												/* eventData2 = column of cell where event was generated */
#define EVENT_ACTIVE_CELL_CHANGE			28
#define EVENT_ROW_SIZE_CHANGE				29  /* eventData1 = row that was sized */
												/* eventData2 = 0 */
#define EVENT_COLUMN_SIZE_CHANGE			30  /* eventData1 = 0 */
												/* eventData2 = column that was sized */
#define EVENT_ACTIVE_TAB_CHANGE				31  /* eventData1 = old active tab */
												/* eventData2 = new active tab */
#define EVENT_EDIT_MODE_STATE_CHANGE		32  /* eventData1 = new edit state */
												/* eventData2 = index */
#define EVENT_MOUSE_POINTER_MOVE			33  /* eventData1 = mouse vertical position */
												/* eventData2 = mouse hoizontal position */
#define EVENT_LEFT_CLICK_UP					34  /* eventData1 = mouse vertical position */
												/* eventData2 = mouse hoizontal position */
#define EVENT_RIGHT_CLICK_UP				35  /* eventData1 = mouse vertical position */
												/* eventData2 = mouse hoizontal position */
#define EVENT_MOUSE_WHEEL_SCROLL			36  /* eventData1 = scroll action (MOUSE_WHEEL_SCROLL_UP, etc) */
												/* eventData2 = amount to scroll */
#define EVENT_PANEL_SIZING					37  /* eventData1 = sizing edge (PANEL_SIZING_TOPLEFT, etc) */
												/* eventData2 = call GetPanelEventRect or SetPanelEventRect to 
													obtain or set value of eventData2 parameter */
#define EVENT_PANEL_MOVING					38  /* eventData1 = not used */
												/* eventData2 = call GetPanelEventRect or SetPanelEventRect to 
													obtain or set value of eventData2 parameter */
#define EVENT_RADIX_CHANGE					39	/* eventData1 = new format */
#define EVENT_ZOOM							40	/* eventData1 = zoom action (ZOOM_IN, etc) */
#define EVENT_AXIS_VAL_CHANGE				41	/* eventData1 = axis that was changed (VAL_BOTTOM_XAXIS, etc) */
#define EVENT_INTERACTIVE_LEGEND			42	/* eventData1 = plot id */
												/* eventData2 = built-in control menu id (VAL_PLOT_COLOR, etc) */
#define EVENT_TABLE_ROW_COL_LABEL_CLICK		43	/* eventData1 = row, eventData2 = col */
#define EVENT_BEGIN_EDIT_TREE_CELL			44  /* eventData1 = row, eventData2 = col */
#define EVENT_TREE_CELL_COMMIT				45  /* eventData1 = row, eventData2 = col */
#define EVENT_TREE_CELL_BEGIN_MENU			46
#define EVENT_TREE_CELL_ACTIVE_ITEM_CHANGE	47  /* eventData1 = row, eventData2 = col */
#define EVENT_TREE_CELL_VAL_CHANGED			48  /* eventData1 = row, eventData2 = col */
#define EVENT_CTRL_MENU						49
#define EVENT_DROP_COPY						50
#define EVENT_VAL_COERCED					51  /* for table events calling the control callback: */
													/* eventData1 = row of cell where event was generated; if 0, event affected multiple cells  */
													/* eventData2 = column of cell where event was generated; if 0, event affected multiple cells  */
#define EVENT_PANEL_MINIMIZE				52
#define EVENT_PANEL_MAXIMIZE				53
#define EVENT_PANEL_RESTORE					54
#define EVENT_RING_BEGIN_MENU				55

//User defined.
#define EVENT_BEFORE_PAINT                              56
#define EVENT_AFTER_PAINT                               57
#define EVENT_LONG_PRESS                                58//长按



/**********************************Attribute******************************************/


#define ATTR_DIMMED                                 500         /* int (boolean) */
#define ATTR_CONSTANT_NAME                          501         /* (char *) (not settable) */
#define ATTR_CONSTANT_NAME_LENGTH                   502         /* int (not settable) */
#define ATTR_CALLBACK_DATA                          510         /* (void *) */
#define ATTR_CALLBACK_NAME                          511         /* (char *) (not settable) */
#define ATTR_CALLBACK_NAME_LENGTH                   512         /* int (not settable) */
#define ATTR_CALLBACK_FUNCTION_POINTER              513         /* (void *) */
#define ATTR_VISIBLE                                530         /* int (boolean) */
#define ATTR_LEFT                                   531         /* int (-32768 - 32767) */
#define ATTR_TOP                                    532         /* int (-32768 - 32767) */
#define ATTR_WIDTH                                  533         /* int (0-32767) */
#define ATTR_HEIGHT                                 540         /* int (0-32767) */
#define ATTR_FRAME_COLOR                            550         /* int (rgb value) */
#define ATTR_SCROLL_BARS                            551         /* int: VAL_NO_SCROLL_BARS, ... */
#define ATTR_SCROLL_BAR_COLOR                       552         /* int (rgb value) */
#define ATTR_HSCROLL_OFFSET                         560         /* int (in pixels) */
#define ATTR_HSCROLL_OFFSET_MAX                     561         /* int (in pixels) */
#define ATTR_CHART_HSCROLL_OFFSET					562			/* int (in points) */
#define ATTR_BACKCOLOR                              570         /* int (rgb value) */
#define ATTR_TITLEBAR_VISIBLE                       571         /* int (boolean) */
#define ATTR_TITLEBAR_THICKNESS                     572         /* int */
#define ATTR_TITLEBAR_ACTUAL_THICKNESS              632         /* int (not settable) */
#define ATTR_TITLE                                  573         /* (char *) */
#define ATTR_TITLE_LENGTH                           574         /* int (not settable) */
#define ATTR_TITLE_COLOR                            575         /* int (rgb value) */
#define ATTR_TITLE_BACKCOLOR                        576         /* int (rgb value) */
#define ATTR_FRAME_STYLE                            577         /* int: VAL_OUTLINED_FRAME, ... */
#define ATTR_FRAME_THICKNESS                        578         /* int */
#define ATTR_FRAME_ACTUAL_WIDTH                     633         /* int (not settable) */
#define ATTR_FRAME_ACTUAL_HEIGHT                    634         /* int (not settable) */
#define ATTR_MENU_HEIGHT                            579         /* int */
#define ATTR_MENU_WIDTH                             635         /* int */
#define ATTR_SIZABLE                                580         /* int (boolean) */
#define ATTR_MOVABLE                                581         /* int (boolean) */
#define ATTR_NUM_CTRLS                              582         /* int (not settable) */
#define ATTR_MOUSE_CURSOR                           583         /* int: VAL_DEFAULT_CURSOR, ... */
#define ATTR_TITLE_FONT                             584         /* (char *) */
#define ATTR_TITLE_CHARACTER_SET                    631         /* int: VAL_ANSI_CHARSET, ... */
#define ATTR_TITLE_ITALIC                           585         /* int (boolean) */
#define ATTR_TITLE_UNDERLINE                        586         /* int (boolean) */
#define ATTR_TITLE_STRIKEOUT                        587         /* int (boolean) */
#define ATTR_TITLE_POINT_SIZE                       588         /* int (1-255) */
#define ATTR_TITLE_BOLD                             589         /* int (boolean) */
#define ATTR_TITLE_FONT_NAME_LENGTH                 590         /* int (not settable) */
#define ATTR_MENU_BAR_VISIBLE                       591         /* int (boolean) */
#define ATTR_CLOSE_CTRL                             592         /* int (ctrl Id) */
#define ATTR_VSCROLL_OFFSET                         593         /* int (in pixels) */
#define ATTR_VSCROLL_OFFSET_MAX                     594         /* int (in pixels) */
#define ATTR_PARENT_SHARES_SHORTCUT_KEYS            595         /* int (boolean) */
#define ATTR_ACTIVATE_WHEN_CLICKED_ON               596         /* int (boolean) */
#define ATTR_WINDOW_ZOOM                            597         /* int: VAL_MINIMIZE, ... */
#define ATTR_SYSTEM_WINDOW_HANDLE                   598         /* intptr_t (HWND, or X-Window) */
#define ATTR_CAN_MINIMIZE                           599         /* int (boolean) */
#define ATTR_CAN_MAXIMIZE                           600         /* int (boolean) */
#define ATTR_CLOSE_ITEM_VISIBLE                     601         /* int (boolean) */
#define ATTR_SYSTEM_MENU_VISIBLE                    602         /* int (boolean) */
#define ATTR_PANEL_MENU_BAR_CONSTANT                603         /* (char *) */
#define ATTR_PANEL_MENU_BAR_CONSTANT_LENGTH         604         /* int (not settable) */
#define ATTR_TITLE_SIZE_TO_FONT                     605         /* int (boolean) */
#define ATTR_PANEL_PARENT                           606         /* int (not settable) */
#define ATTR_NUM_CHILDREN                           607         /* int (not settable) */
#define ATTR_FIRST_CHILD                            608         /* int (not settable) */
#define ATTR_NEXT_PANEL                             609         /* int (not settable) */
#define ATTR_ZPLANE_POSITION                        610         /* int (0 - numObjects-1) */
#define ATTR_CTRL_STYLE                             611         /* int: CTRL_NUMERIC, ... (not settable) */
#define ATTR_CTRL_TAB_POSITION                      612         /* int */
#define ATTR_PANEL_FIRST_CTRL                       612         /* int (not settable) */
#define ATTR_NEXT_CTRL                              613         /* int (not settable) */
#define ATTR_ACTIVE                                 614         /* int (not settable) */
#define ATTR_FLOATING                               615         /* int VAL_FLOAT_NEVER, ... */
#define ATTR_TL_ACTIVATE_WHEN_CLICKED_ON            616         /* int (boolean) */
#define ATTR_CONFORM_TO_SYSTEM                      617         /* int (boolean) */
#define ATTR_SCALE_CONTENTS_ON_RESIZE               618         /* int (boolean) */
#define ATTR_MIN_HEIGHT_FOR_SCALING                 619         /* int (0-32767) */
#define ATTR_CTRL_MODE                              620         /* int: VAL_HOT, ... */
#define ATTR_MIN_WIDTH_FOR_SCALING                  621         /* int (0-32767) */
#define ATTR_HAS_TASKBAR_BUTTON                     622         /* int (boolean) */
#define ATTR_OWNER_THREAD_ID                        623         /* int (not settable) */
#define ATTR_SCROLL_BAR_STYLE                       624			/* int: VAL_CLASSIC, ... */
#define ATTR_MINIMIZE_OTHER_PANELS                  625			/* int (boolean) */
#define ATTR_CTRL_VAL                               630         /* same datatype as ctrl */
#define ATTR_LABEL_COLOR                            640         /* int (rgb value) */
#define ATTR_LABEL_TEXT                             641         /* (char *) */
#define ATTR_LABEL_FONT                             642         /* (char *) */
#define ATTR_LABEL_ITALIC                           643         /* int (boolean) */
#define ATTR_LABEL_UNDERLINE                        644         /* int (boolean) */
#define ATTR_LABEL_STRIKEOUT                        645         /* int (boolean) */
#define ATTR_LABEL_POINT_SIZE                       646         /* int (1-255) */
#define ATTR_LABEL_BOLD                             647         /* int (boolean) */
#define ATTR_LABEL_ANGLE                            651         /* int (-32768 - 32767) */
#define ATTR_LABEL_TEXT_LENGTH                      648         /* int (not settable) */
#define ATTR_LABEL_SIZE_TO_TEXT                     649         /* int (boolean) */
#define ATTR_LABEL_FONT_NAME_LENGTH                 650         /* int (not settable) */
#define ATTR_LABEL_CHARACTER_SET                    661         /* int: VAL_ANSI_CHARSET, ... */
#define ATTR_LABEL_VISIBLE                          660         /* int (boolean) */
#define ATTR_LABEL_LEFT                             670         /* int (-32768 - 32767) */
#define ATTR_LABEL_TOP                              671         /* int (-32768 - 32767) */
#define ATTR_LABEL_WIDTH                            672         /* int (0-32767) */
#define ATTR_LABEL_HEIGHT                           673         /* int (0-32767) */
#define ATTR_LABEL_BGCOLOR                          674         /* int (rgb value) */
#define ATTR_LABEL_JUSTIFY                          675         /* int: VAL_LEFT_JUSTIFIED, ... */
#define ATTR_LABEL_RAISED                           676         /* int (boolean) */
#define ATTR_TOOLTIP_TEXT                           652         /* (char *) */
#define ATTR_TOOLTIP_TEXT_LENGTH                    653         /* int (not settable) */
#define ATTR_TOOLTIP_DELAY							654			/* int (0-10000) */
#define ATTR_DISABLE_CTRL_TOOLTIP                   655			/* int (boolean) */
#define ATTR_TOOLTIP_HIDE_DELAY						656			/* int (>=0) */
#define ATTR_TEXT_COLOR                             680         /* int (rgb value) */
#define ATTR_TEXT_FONT_NAME_LENGTH                  681         /* int (not settable) */
#define ATTR_TEXT_FONT                              682         /* (char *) */
#define ATTR_TEXT_ITALIC                            683         /* int (boolean) */
#define ATTR_TEXT_UNDERLINE                         684         /* int (boolean) */
#define ATTR_TEXT_STRIKEOUT                         685         /* int (boolean) */
#define ATTR_TEXT_POINT_SIZE                        686         /* int (1-255) */
#define ATTR_TEXT_BOLD                              687         /* int (boolean) */
#define ATTR_TEXT_ANGLE                             688         /* int (-32768 - 32767) */
#define ATTR_TEXT_CHARACTER_SET                     689         /* int: VAL_ANSI_CHARSET, ... */
#define ATTR_TEXT_BGCOLOR                           690         /* int (rgb value) */
#define ATTR_TEXT_JUSTIFY                           700         /* int: VAL_LEFT_JUSTIFIED, ... */
#define ATTR_DATA_TYPE                              710         /* int: VAL_CHAR, ... */
#define ATTR_CTRL_INDEX                             720         /* int: (0 to numItems-1) */
#define ATTR_DFLT_INDEX                             721         /* int: (0 to numItems-1) */
#define ATTR_MAX_VALUE                              730         /* same datatype as ctrl */
#define ATTR_MIN_VALUE                              731         /* same datatype as ctrl */
#define ATTR_DFLT_VALUE                             732         /* same datatype as ctrl */
#define ATTR_INCR_VALUE                             733         /* same datatype as ctrl */
#define ATTR_FORMAT                                 734         /* int: VAL_HEX_FORMAT, ... */
#define ATTR_PRECISION                              735         /* int (0 - 32) */
#define ATTR_SHOW_RADIX                             736         /* int (boolean) */
#define ATTR_SHOW_INCDEC_ARROWS                     737         /* int (boolean) */
#define ATTR_CHECK_RANGE                            738         /* int: VAL_COERCE, ... */
#define ATTR_PADDING                                739         /* int: (0 - 64) */
#define ATTR_DFLT_VALUE_LENGTH                      740         /* int (not settable) */
#define ATTR_DISABLE_RADIX                          741         /* int (boolean) */
#define ATTR_INCDEC_WIDTH                           742         /* int (0-32767) */
#define ATTR_SLIDE_INCDEC_LENGTH                    743			/* int (0-32767) */
#define ATTR_MENU_ARROW_WIDTH                       744			/* int (0-32767) */
#define ATTR_MENU_ARROW_HEIGHT                      745			/* int (0-32767) */
#define ATTR_DISABLE_CHECK_MARK                     755         /* int (boolean) */
#define ATTR_MAX_ENTRY_LENGTH                       760         /* int (-1 means no limit) */
#define ATTR_MAX_ENTRY_CHARS                        761         /* int (-1 means no limit) */
#define ATTR_TEXT_SELECTION_START                   762         /* int */
#define ATTR_TEXT_SELECTION_LENGTH                  763         /* int */
#define ATTR_ENABLE_CHARACTER_MASKING				764			/* int (boolean) */
#define ATTR_MASK_CHARACTER							765			/* int (ASCII character) */
#define ATTR_STRING_TEXT_LENGTH                     770         /* int (not settable) */
#define ATTR_FIRST_VISIBLE_LINE                     790         /* int */
#define ATTR_WRAP_MODE                              791         /* int: VAL_CHAR_WRAP, ... */
#define ATTR_EXTRA_LINES                            793         /* int (-1 means no limit) */
#define ATTR_TOTAL_LINES                            794         /* int (not settable) */
#define ATTR_ENTER_IS_NEWLINE                       795         /* int (boolean) */
#define ATTR_CONFORM_TO_SYSTEM_THEME                800			/* int (boolean) */
#define ATTR_DISABLE_PANEL_THEME                    801			/* int (boolean) */
#define ATTR_SCROLL_BAR_SIZE                        820         /* int: VAL_SMALL_SCROLL_BARS, ... */
#define ATTR_VISIBLE_LINES                          821         /* int (1 - 500) */
#define ATTR_NO_EDIT_TEXT                           840         /* int (boolean) */
#define ATTR_TEXT_RAISED                            860         /* int (boolean) */
#define ATTR_SIZE_TO_TEXT                           861         /* int (boolean) */
#define ATTR_CMD_BUTTON_COLOR                       880         /* int (rgb value) */
#define ATTR_AUTO_SIZING                            881         /* int: VAL_ALWAYS_AUTO_SIZE, ... */
#define ATTR_ON_VALUE                               900         /* same datatype as ctrl */
#define ATTR_OFF_VALUE                              901         /* same datatype as ctrl */
#define ATTR_ON_VALUE_LENGTH                        902         /* int (only if string value) (not settable) */
#define ATTR_OFF_VALUE_LENGTH                       903         /* int (only if string value) (not settable) */
#define ATTR_BINARY_SWITCH_COLOR                    904         /* int (rgb value) */
#define ATTR_ON_COLOR                               920         /* int (rgb value) */
#define ATTR_OFF_COLOR                              921         /* int (rgb value) */
#define ATTR_ON_TEXT                                940         /* (char *) */
#define ATTR_OFF_TEXT                               941         /* (char *) */
#define ATTR_ON_TEXT_LENGTH                         942         /* int (not settable) */
#define ATTR_OFF_TEXT_LENGTH                        943         /* int (not settable) */
#define ATTR_DIG_DISP_TOP                           970         /* int (-32768 - 32767) */
#define ATTR_DIG_DISP_LEFT                          971         /* int (-32768 - 32767) */
#define ATTR_DIG_DISP_WIDTH                         972         /* int (0-32767) */
#define ATTR_DIG_DISP_HEIGHT                        973         /* int (0-32767) */
#define ATTR_SHOW_DIG_DISP                          974         /* int (boolean) */
#define ATTR_SLIDER_COLOR                           980         /* int (rgb value) */
#define ATTR_NEEDLE_COLOR                           ATTR_SLIDER_COLOR
#define ATTR_FILL_HOUSING_COLOR                     981         /* int (rgb value) */
#define ATTR_MARKER_STYLE                           982         /* int: VAL_NO_MARKERS, ... */
#define ATTR_TICK_STYLE                             983         /* int: VAL_NO_TICKS, ... */
#define ATTR_FILL_COLOR                             985         /* int (rgb value) */
#define ATTR_FILL_OPTION                            986         /* int: VAL_NO_FILL, ... */
#define ATTR_MARKER_START_ANGLE                     990         /* int (0-359) */
#define ATTR_MARKER_END_ANGLE                       991         /* int (0-359) */
#define ATTR_SLIDER_WIDTH                           992         /* int (0-32767) */
#define ATTR_SLIDER_HEIGHT                          993         /* int (0-32767) */
#define ATTR_FORCE_SMOOTH_DRAGGING					994			/* int (boolean) */
#define ATTR_SHOW_MORE_BUTTON                       995         /* int (boolean) */
#define ATTR_SHOW_TRANSPARENT                       996         /* int (boolean) */
#define ATTR_SLIDER_LEFT                            997         /* int (0-32767) */
#define ATTR_SLIDER_TOP                             998         /* int (0-32767) */
#define ATTR_NUM_DIVISIONS                          999			/* int (1-100, or VAL_AUTO) */
#define ATTR_MENU_ARROW_COLOR                       1000        /* int (rgb value) */
#define ATTR_MENU_BAR_POINT_SIZE                    1010        /* int (1-255) */
#define ATTR_MENU_BAR_BOLD                          1011        /* int (boolean) */
#define ATTR_MENU_BAR_ITALIC                        1012        /* int (boolean) */
#define ATTR_MENU_BAR_UNDERLINE                     1013        /* int (boolean) */
#define ATTR_MENU_BAR_STRIKEOUT                     1014        /* int (boolean) */
#define ATTR_MENU_BAR_FONT                          1015		/* (char *) */
#define ATTR_MENU_BAR_FONT_NAME_LENGTH              1016		/* int (not settable) */
#define ATTR_MENU_BAR_CHARACTER_SET                 1021        /* int: VAL_ANSI_CHARSET, ... */
#define	ATTR_MENU_IMAGE_BACKGROUND_COLOR            1017	    /* int (rgb value) */
#define ATTR_MENU_BAR_IMAGE_SIZE	                1018		/* int (not settable) */
#define ATTR_SHOW_IMMEDIATE_ACTION_SYMBOL           1019	    /* int (boolean) */
#define ATTR_SHORTCUT_KEY                           1020        /* int */
#define ATTR_CHECKED                                1040        /* int (boolean) */
#define ATTR_IS_SEPARATOR                           1041        /* int (boolean) (not settable) */
#define ATTR_ITEM_NAME                              1042        /* (char *) */
#define ATTR_ITEM_NAME_LENGTH                       1043        /* int (not settable) */
#define ATTR_SUBMENU_ID                             1044        /* int (not settable) */
#define ATTR_NEXT_ITEM_ID                           1045        /* int (not settable) */
#define ATTR_BOLD                                   1046        /* int (boolean) */
#define ATTR_IMAGE_FILE                             1050        /* (char *) */
#define ATTR_IMAGE_FILE_LENGTH                      1051        /* int (not settable) */
#define ATTR_FAST_DRAW_BUTTON                       1052        /* int (boolean) */
#define ATTR_USE_SUBIMAGE                           1053        /* int (boolean) */
#define ATTR_SUBIMAGE_TOP                           1054        /* int (positive or zero) */
#define ATTR_SUBIMAGE_LEFT                          1055        /* int (positive or zero) */
#define ATTR_SUBIMAGE_WIDTH                         1056        /* int (positive or zero) */
#define ATTR_SUBIMAGE_HEIGHT                        1057        /* int (positive or zero) */
#define ATTR_MENU_NAME                              1060        /* (char *) */
#define ATTR_MENU_NAME_LENGTH                       1061        /* int (not settable) */
#define ATTR_NUM_MENU_ITEMS                         1062        /* int (not settable) */
#define ATTR_NEXT_MENU_ID                           1063        /* int (not settable) */
#define ATTR_FIRST_ITEM_ID                          1064        /* int (not settable) */
#define ATTR_NUM_MENUS                              1070        /* int (not settable) */
#define ATTR_DRAW_LIGHT_BEVEL                       1071        /* int (boolean) */
#define ATTR_DIMMER_CALLBACK                        1072        /* (void *) (really a MenuDimmerCallbackPtr) */
#define ATTR_FIRST_MENU_ID                          1073        /* int (not settable) */
#define ATTR_SEND_DIMMER_EVENTS_FOR_ALL_KEYS        1074        /* int (boolean) */
#define ATTR_GRID_COLOR                             1080        /* int (rgb value) */
#define ATTR_GRID_STYLE                             1079        /* int: the same values as the pen style ones */
#define ATTR_PLOT_BGCOLOR                           1081        /* int (rgb value) */
#define ATTR_XYNAME_FONT                            1082        /* (char *) */
#define ATTR_XYNAME_CHARACTER_SET                   1138        /* int: VAL_ANSI_CHARSET, ... */
#define ATTR_XYNAME_COLOR                           1083        /* int (rgb value) */
#define ATTR_XYLABEL_FONT                           1084        /* (char *) */
#define ATTR_XYLABEL_COLOR                          1085        /* int (rgb value) */
#define ATTR_XNAME                                  1086        /* (char *) */
#define ATTR_XGRID_VISIBLE                          1087        /* int (boolean) */
#define ATTR_XLABEL_VISIBLE                         1088        /* int (boolean) */
#define ATTR_XFORMAT                                1089        /* int: VAL_ENGINEERING... */
#define ATTR_XDIVISIONS                             1090        /* int (1-100, or VAL_AUTO) */
#define ATTR_XPRECISION                             1091        /* int (0-15, or VAL_AUTO) */
#define ATTR_XENG_UNITS                             1092        /* int (-308 to 308) */
#define ATTR_YNAME                                  1093        /* (char *) */
#define ATTR_YGRID_VISIBLE                          1094        /* int (boolean) */
#define ATTR_YLABEL_VISIBLE                         1095        /* int (boolean) */
#define ATTR_YMAP_MODE                              1096        /* int: VAL_LINEAR or VAL_LOG */
#define ATTR_YFORMAT                                1097        /* int: VAL_ENGINEERING... */
#define ATTR_YDIVISIONS                             1098        /* int (1-100, or VAL_AUTO) */
#define ATTR_YPRECISION                             1099        /* int (0-15, or VAL_AUTO) */
#define ATTR_YENG_UNITS                             1100        /* int (-308 to 308) */
#define ATTR_EDGE_STYLE                             1101        /* int: VAL_RAISED_EDGE, ...*/
#define ATTR_BORDER_VISIBLE                         1102        /* int (boolean) */
#define ATTR_XYNAME_BOLD                            1103        /* int (boolean) */
#define ATTR_XYNAME_ITALIC                          1104        /* int (boolean) */
#define ATTR_XYNAME_UNDERLINE                       1105        /* int (boolean) */
#define ATTR_XYNAME_STRIKEOUT                       1106        /* int (boolean) */
#define ATTR_XYNAME_POINT_SIZE                      1107        /* int (1-255) */
#define ATTR_XNAME_LENGTH                           1108        /* int (not settable) */
#define ATTR_YNAME_LENGTH                           1109        /* int (not settable) */
#define ATTR_XYNAME_FONT_NAME_LENGTH                1110        /* int (not settable) */
#define ATTR_XYLABEL_BOLD                           1111        /* int (boolean) */
#define ATTR_XYLABEL_ITALIC                         1112        /* int (boolean) */
#define ATTR_XYLABEL_UNDERLINE                      1113        /* int (boolean) */
#define ATTR_XYLABEL_STRIKEOUT                      1114        /* int (boolean) */
#define ATTR_XYLABEL_POINT_SIZE                     1115        /* int (1-255) */
#define ATTR_XYLABEL_FONT_NAME_LENGTH               1116        /* int (not settable) */
#define ATTR_GRAPH_BGCOLOR                          1117        /* int (rgb value) */
#define ATTR_PLOT_AREA_WIDTH                        1118        /* int (not settable) */
#define ATTR_PLOT_AREA_HEIGHT                       1119        /* int (not settable) */
#define ATTR_INNER_MARKERS_VISIBLE                  1120        /* int (boolean) */
#define ATTR_YREVERSE                               1121        /* int (boolean) */
#define ATTR_XUSE_LABEL_STRINGS                     1122        /* int (boolean) */
#define ATTR_YUSE_LABEL_STRINGS                     1123        /* int (boolean) */
#define ATTR_XAXIS_GAIN                             1124        /* double */
#define ATTR_YAXIS_GAIN                             1125        /* double */
#define ATTR_XAXIS_OFFSET                           1126        /* double */
#define ATTR_YAXIS_OFFSET                           1127        /* double */
#define ATTR_PLOT_AREA_TOP                          1128        /* int (not settable) */
#define ATTR_PLOT_AREA_LEFT                         1129        /* int (not settable) */
#define ATTR_XPADDING                               1130        /* int (0 to 64) */
#define ATTR_YPADDING                               1131        /* int (0 to 64) */
#define ATTR_ACTUAL_XDIVISIONS                      1132        /* int (not settable) */
#define ATTR_ACTUAL_YDIVISIONS                      1133        /* int (not settable) */
#define ATTR_ACTUAL_XPRECISION                      1134        /* int (not settable) */
#define ATTR_ACTUAL_YPRECISION                      1135        /* int (not settable) */
#define ATTR_XMINORGRID_VISIBLE                     1136        /* int (boolean) */
#define ATTR_YMINORGRID_VISIBLE                     1137        /* int (boolean) */
#define ATTR_FIXED_PLOT_AREA						1139		/* int (boolean) */
#define ATTR_NUM_CURSORS                            1140        /* int (0-10) */
#define ATTR_XMAP_MODE                              1141        /* int: VAL_LINEAR or VAL_LOG */
#define ATTR_DATA_MODE                              1142        /* int: VAL_RETAIN, ... */
#define ATTR_COPY_ORIGINAL_DATA                     1143        /* int (boolean) */
#define ATTR_XMARK_ORIGIN                           1144        /* int (boolean) */
#define ATTR_YMARK_ORIGIN                           1145        /* int (boolean) */
#define ATTR_SMOOTH_UPDATE                          1146        /* int (boolean) */
#define ATTR_REFRESH_GRAPH                          1147        /* int (boolean) */
#define ATTR_SHIFT_TEXT_PLOTS                       1148        /* int (boolean) */
#define ATTR_ACTIVE_YAXIS                           1149        /* int: VAL_LEFT_YAXIS or VAL_RIGHT_YAXIS */
#define ATTR_XREVERSE                               1150        /* int (boolean) */
#define ATTR_ENABLE_ZOOM_AND_PAN                    1151        /* int (boolean) */
#define ATTR_XLOOSE_FIT_AUTOSCALING                 1152        /* int (boolean) */
#define ATTR_YLOOSE_FIT_AUTOSCALING                 1153        /* int (boolean) */
#define ATTR_XLOOSE_FIT_AUTOSCALING_UNIT            1154        /* int (-308 to 308) */
#define ATTR_YLOOSE_FIT_AUTOSCALING_UNIT            1155        /* int (-308 to 308) */
#define ATTR_ENABLE_ANTI_ALIASING                   1156        /* int (boolean) */
#define ATTR_LEGEND_VISIBLE                         1157        /* int (boolean) */
#define ATTR_LEGEND_TOP                             1158        /* int (-32768 - 32767) */
#define ATTR_LEGEND_LEFT                            1159        /* int (-32768 - 32767) */
#define ATTR_LEGEND_WIDTH                           1160        /* int (-32768 - 32767) */
#define ATTR_LEGEND_HEIGHT                          1161        /* int (-32768 - 32767) */
#define ATTR_LEGEND_FRAME_COLOR                     1162        /* int (rgb value) */
#define ATTR_LEGEND_PLOT_BGCOLOR                    1163        /* int (rgb value) */
#define ATTR_LEGEND_SHOW_SAMPLES                    1164        /* int (boolean) */
#define ATTR_LEGEND_AUTO_SIZE                       1165        /* int (boolean) */
#define ATTR_LEGEND_AUTO_DISPLAY                    1166        /* int (boolean) */
#define ATTR_LEGEND_NUM_VISIBLE_ITEMS               1167        /* int (0-500) */
#define ATTR_ACTIVE_XAXIS                           1168		/* int: VAL_BOTTOM_XAXIS or VAL_TOP_XAXIS */
#define ATTR_LEGEND_INTERACTIVE                     1169		/* int (boolean) */
#define ATTR_NUM_TRACES                             1170        /* int (1-64) */
#define ATTR_POINTS_PER_SCREEN                      1171        /* int (3-10000) */
#define ATTR_SCROLL_MODE                            1172        /* int: VAL_SWEEP, ... */
#define ATTR_ENABLE_EDITABLE_AXES                   1173		/* int (boolean) */
#define ATTR_ZOOM_STYLE								1180		/* int: ZOOM_AROUND_PT, ... */
#define ATTR_CURSOR_MODE                            1200        /* int: VAL_FREE_FORM, ... */
#define ATTR_CURSOR_POINT_STYLE                     1201        /* int: VAL_EMPTY_SQUARE, ... */
#define ATTR_CROSS_HAIR_STYLE                       1202        /* int: VAL_LONG_CROSS, ... */
#define ATTR_CURSOR_COLOR                           1203        /* int (rgb value) */
#define ATTR_CURSOR_YAXIS                           1204        /* int: VAL_LEFT_YAXIS or VAL_RIGHT_YAXIS */
#define ATTR_CURSOR_ENABLED                         1205        /* int (boolean) */
#define ATTR_CURSOR_XAXIS                           1206		/* int: VAL_BOTTOM_XAXIS or VAL_TOP_XAXIS */
#define ATTR_CURSOR_SNAPPING_PLOT                   1207        /* int (plot Id) */
#define ATTR_PLOT_OPACITY							1228		/* int (0-255) */
#define ATTR_TRACE_OPACITY							ATTR_PLOT_OPACITY	/* int (0-255) */
#define ATTR_TRACE_THICKNESS						1229		/* int (1-32) */
#define ATTR_TRACE_COLOR                            1230        /* int (rgb value) */
#define ATTR_PLOT_STYLE                             1231        /* int: VAL_THIN_LINE, ... */
#define ATTR_TRACE_POINT_STYLE                      1232        /* int: VAL_EMPTY_SQUARE, ... */
#define ATTR_LINE_STYLE                             1233        /* int: VAL_SOLID, ... */
#define ATTR_TRACE_VISIBLE                          1234        /* int (boolean) */
#define ATTR_TRACE_YAXIS							1235		/* int: VAL_LEFT_YAXIS or VAL_RIGHT_YAXIS */
#define ATTR_HISTORY_BUFFER_SIZE					1236		/* int: >= pointsPerScreen */
#define ATTR_STRIP_CHART_PAUSED						1237		/* int (boolean) */
#define ATTR_SWEEP_LINE_COLOR						1238		/* int (rgb value) */
#define ATTR_SHOW_CHART_DIVISION_LABELS				1239		/* int (boolean) */
#define ATTR_TRACE_BGCOLOR                          1240        /* int (rgb value) */
#define ATTR_PLOT_FONT                              1241        /* (char *) */
#define ATTR_PLOT_FONT_NAME_LENGTH                  1242        /* int (not settable) */
#define ATTR_INTERPOLATE_PIXELS                     1243        /* int (boolean) */
#define ATTR_PLOT_ZPLANE_POSITION                   1244        /* int (0 - numPlots - 1) */
#define ATTR_NUM_POINTS                             1245        /* size_t (not settable) */
#define ATTR_PLOT_XDATA                             1246        /* (void *) */
#define ATTR_PLOT_YDATA                             1247        /* (void *) */
#define ATTR_PLOT_ZDATA                             1248        /* (void *) */
#define ATTR_PLOT_XDATA_TYPE                        1249        /* int (not settable) */
#define ATTR_PLOT_YDATA_TYPE                        1250        /* int (not settable) */
#define ATTR_PLOT_ZDATA_TYPE                        1251        /* int (not settable) */
#define ATTR_PLOT_XDATA_SIZE                        1252        /* size_t (not settable) */
#define ATTR_PLOT_YDATA_SIZE                        1253        /* size_t (not settable) */
#define ATTR_PLOT_ZDATA_SIZE                        1254        /* size_t (not settable) */
#define ATTR_PLOT_YAXIS                             1255        /* int: VAL_LEFT_YAXIS or VAL_RIGHT_YAXIS */
#define ATTR_PLOT_SNAPPABLE                         1256        /* int (boolean) */
#define ATTR_PLOT_ORIGIN                            1257        /* int: VAL_LOWER_LEFT,... */
#define ATTR_PLOT_THICKNESS                         1258        /* int: 1-32 (only for solid lines) */
#define ATTR_PLOT_XAXIS                             1259		/* int: VAL_BOTTOM_XAXIS or VAL_TOP_XAXIS */
#define ATTR_CHECK_MODE                             1260        /* int (boolean) */
#define ATTR_CHECK_STYLE                            1261        /* int: VAL_CHECK_MARK, ... */
#define ATTR_TEXT_CLICK_TOGGLES_CHECK               1262        /* int: (boolean) */
#define ATTR_HILITE_CURRENT_ITEM                    1263        /* int (boolean) */
#define ATTR_ALLOW_ROOM_FOR_IMAGES                  1264        /* int (boolean) */
#define ATTR_DRAGGABLE_MARKS                        1265        /* int (boolean) */
#define ATTR_INTERVAL                               1270        /* double (seconds) */
#define ATTR_ENABLED                                1271        /* int (boolean) */
#define ATTR_PLOT_LG_VISIBLE                        1275        /* int (boolean) */
#define ATTR_TRACE_LG_VISIBLE						ATTR_PLOT_LG_VISIBLE
#define ATTR_PLOT_LG_TEXT                           1276        /* (char *) */
#define ATTR_TRACE_LG_TEXT                          ATTR_PLOT_LG_TEXT
#define ATTR_PLOT_LG_TEXT_LENGTH                    1277        /* int (not settable) */
#define ATTR_TRACE_LG_TEXT_LENGTH                   ATTR_PLOT_LG_TEXT_LENGTH
#define ATTR_PLOT_LG_TEXT_COLOR                     1278        /* int (rgb value) */
#define ATTR_TRACE_LG_TEXT_COLOR                    ATTR_PLOT_LG_TEXT_COLOR
#define ATTR_PLOT_LG_FONT                           1280        /* (char *) */
#define ATTR_TRACE_LG_FONT                          ATTR_PLOT_LG_FONT
#define ATTR_PLOT_LG_FONT_NAME_LENGTH               1281        /* int (not settable) */
#define ATTR_TRACE_LG_FONT_NAME_LENGTH              ATTR_PLOT_LG_FONT_NAME_LENGTH
#define ATTR_FRAME_VISIBLE                          1290        /* int (boolean) */
#define ATTR_PICT_BGCOLOR                           1291        /* int (rgb value) */
#define ATTR_FIT_MODE                               1292        /* int: VAL_SIZE_TO_IMAGE, ... */
#define ATTR_ORIENTATION                            1300        /* int: VAL_PORTRAIT, ... */
#define ATTR_PRINT_AREA_HEIGHT                      1301        /* int (millimeter/10), VAL_USE... */
#define ATTR_PRINT_AREA_WIDTH                       1302        /* int (millimeter/10), VAL_USE... */
#define ATTR_NUMCOPIES                              1303        /* int (1-5000) */
#define ATTR_XRESOLUTION                            1304        /* int (dpi), ... */
#define ATTR_YRESOLUTION                            1305        /* int (dpi), ... */
#define ATTR_XOFFSET                                1306        /* int (millimeter/10), VAL_CENTER... */
#define ATTR_YOFFSET                                1307        /* int (millimeter/10), VAL_CENTER... */
#define ATTR_COLOR_MODE                             1308        /* int: VAL_BW, ... */
#define ATTR_DUPLEX                                 1309        /* int: VAL_SIMPLEX, ... */
#define ATTR_EJECT_AFTER                            1310        /* int (boolean) */
#define ATTR_TEXT_WRAP                              1311        /* int (boolean) */
#define ATTR_TAB_INTERVAL                           1312        /* int (1-80) */
#define ATTR_SHOW_PAGE_NUMBERS                      1313        /* int (boolean) */
#define ATTR_SHOW_LINE_NUMBERS                      1314        /* int (boolean) */
#define ATTR_SHOW_FILE_NAME                         1315        /* int (boolean) */
#define ATTR_SHOW_DATE                              1316        /* int (boolean) */
#define ATTR_SHOW_TIME                              1317        /* int (boolean) */
#define ATTR_PRINT_FONT_NAME                        1318        /* (char *) */
#define ATTR_PRINT_ITALIC                           1319        /* int (boolean) */
#define ATTR_PRINT_UNDERLINE                        1320        /* int (boolean) */
#define ATTR_PRINT_STRIKEOUT                        1321        /* int (boolean) */
#define ATTR_PRINT_POINT_SIZE                       1322        /* int (6-48) */
#define ATTR_PRINT_BOLD                             1323        /* int (boolean) */
#define ATTR_PRINT_FONT_NAME_LENGTH                 1324        /* int (not settable) */
#define ATTR_PRINTER_NAME                           1325        /* (char *) */
#define ATTR_PRINTER_NAME_LENGTH                    1326        /* int (not settable) */
#define ATTR_BITMAP_PRINTING                        1327        /* int (boolean) */
#define ATTR_SYSTEM_PRINT_DIALOG_ONLY               1328        /* int (boolean) */
#define ATTR_CHARS_PER_LINE                         1329        /* int */
#define ATTR_LINES_PER_PAGE                         1330        /* int */
#define ATTR_PRINT_CHARACTER_SET                    1331        /* int: VAL_NATIVE_CHARSET, ... */
#define ATTR_PEN_COLOR                              1350        /* int (rgb value) */
#define ATTR_PEN_FILL_COLOR                         1351        /* int (rgb value) */
#define ATTR_PEN_MODE                               1354        /* int: VAL_COPY_MODE, ... */
#define ATTR_PEN_WIDTH                              1355        /* int: (1-255) */
#define ATTR_PEN_PATTERN                            1356        /* unsigned char[8] */
#define ATTR_PEN_STYLE                              1357        /* int: VAL_SOLID, ... */
#define ATTR_DRAW_POLICY                            1370        /* int: VAL_UPDATE_IMMEDIATELY, ... */
#define ATTR_OVERLAPPED                             1372        /* int (not settable) */
#define ATTR_OVERLAPPED_POLICY                      1373        /* int: VAL_DRAW_ON_TOP or VAL_DEFER_DRAWING */
#define ATTR_XCOORD_AT_ORIGIN                       1374        /* double */
#define ATTR_YCOORD_AT_ORIGIN                       1375        /* double */
#define ATTR_XSCALING                               1376        /* double */
#define ATTR_YSCALING                               1377        /* double */
#define ATTR_ALLOW_UNSAFE_TIMER_EVENTS              1400        /* int (boolean) */
#define ATTR_REPORT_LOAD_FAILURE                    1405        /* int (boolean) */
#define ATTR_ALLOW_MISSING_CALLBACKS                1410        /* int (boolean) */
#define ATTR_ANTI_ALIASING_RESOLUTION               1411        /* int (boolean) */
#define ATTR_SUPPRESS_EVENT_PROCESSING              1415        /* int (boolean) */
#define ATTR_TASKBAR_BUTTON_VISIBLE                 1420        /* int (boolean) */
#define ATTR_TASKBAR_BUTTON_TEXT                    1425        /* (char *)      */
#define ATTR_DEFAULT_MONITOR                        1430        /* int */
#define ATTR_PRIMARY_MONITOR                        1435        /* int */
#define ATTR_NUM_MONITORS                           1440        /* int */
#define ATTR_FIRST_MONITOR                          1445        /* int */
#define ATTR_DISABLE_PROG_PANEL_SIZE_EVENTS         1450        /* int (boolean) */
#define ATTR_USE_LOCALIZED_DECIMAL_SYMBOL           1451        /* int: VAL_USE_PERIOD, VAL_USE_SYSTEM_SETTING */
#define ATTR_LOCALIZED_DECIMAL_SYMBOL               1452        /* char (not settable) */
#define ATTR_NUM_DIGITS_FLOATING_POINT              1453        /* int */
#define ATTR_MINIMIZE_PANELS_TO_DESKTOP             1455        /* int (boolean) */
#define ATTR_USE_CHECKERBOARD_DIMMING               1456		/* int (boolean) */
#define ATTR_RESOLUTION_ADJUSTMENT                  1500        /* int: VAL_USE_PANEL_SETTING, 0-100 */
#define ATTR_UPPER_LEFT_CORNER_COLOR                1510        /* int (rgb value) */
#define ATTR_ROW_LABELS_COLOR                       1511        /* int (rgb value) */
#define ATTR_COLUMN_LABELS_COLOR                    1512        /* int (rgb value) */
#define ATTR_TABLE_BGCOLOR                          1513        /* int (rgb value) */
#define ATTR_TABLE_MODE                             1514        /* int: VAL_COLUMN, ... */
#define ATTR_ROW_LABELS_VISIBLE                     1515        /* int (boolean) */
#define ATTR_COLUMN_LABELS_VISIBLE                  1516        /* int (boolean) */
#define ATTR_ROW_LABELS_WIDTH                       1517        /* int (0-32767) */
#define ATTR_COLUMN_LABELS_HEIGHT                   1518        /* int (0-32767) */
#define ATTR_FIRST_VISIBLE_ROW                      1519        /* int */
#define ATTR_FIRST_VISIBLE_COLUMN                   1520        /* int */
#define ATTR_NUM_VISIBLE_ROWS                       1521        /* int */
#define ATTR_NUM_VISIBLE_COLUMNS                    1522        /* int */
#define ATTR_ENABLE_ROW_SIZING				        1523        /* int (boolean) */
#define ATTR_ENABLE_COLUMN_SIZING                   1524        /* int (boolean) */
#define ATTR_ENABLE_POPUP_MENU                      1525        /* int (boolean) */
#define ATTR_GRID_AREA_TOP                          1526        /* int (not settable) */
#define ATTR_GRID_AREA_LEFT                         1527        /* int (not settable) */
#define ATTR_GRID_AREA_WIDTH                        1528        /* int (not settable) */
#define ATTR_GRID_AREA_HEIGHT                       1529        /* int (not settable) */
#define ATTR_TABLE_RUN_STATE                        1530        /* int: VAL_SELECT_STATE, ... */
#define ATTR_AUTO_EDIT                              1531        /* int (boolean) */
#define ATTR_SNAP_COLUMN_WIDTHS_TO_CTRL_WIDTH       1532        /* int (boolean) */
#define ATTR_CELL_TYPE                              1580        /* int: VAL_CELL_NUMERIC, ... */
#define ATTR_CELL_DIMMED                            1581        /* int (boolean) */
#define ATTR_CELL_MODE                              1582        /* int: VAL_HOT, ... */
#define ATTR_HORIZONTAL_GRID_COLOR                  1583        /* int (rgb value) */
#define ATTR_VERTICAL_GRID_COLOR                    1584        /* int (rgb value) */
#define ATTR_HORIZONTAL_GRID_VISIBLE                1585        /* int (boolean) */
#define ATTR_VERTICAL_GRID_VISIBLE                  1586        /* int (boolean) */
#define ATTR_MIN_NUM_LINES_VISIBLE                  1587        /* int */
#define ATTR_INCDEC_ARROW_COLOR                     1588        /* int (rgb value) */
#define ATTR_NUM_CELL_DFLT_VALUE                    1589        /* same datatype as cell */
#define ATTR_STR_CELL_DFLT_VALUE                    1590        /* (char *) */
#define ATTR_STR_CELL_DFLT_VALUE_LENGTH             1591        /* int (not settable) */
#define ATTR_CELL_JUSTIFY                           1592        /* int: VAL_TOP_LEFT_JUSTIFIED, ... */
#define ATTR_STR_CELL_NUM_LINES                     1593		/* int (not settable) */
#define ATTR_CELL_FRAME_COLOR                       1594        /* int (rgb value) */
#define ATTR_SHOW_RING_ARROW                        1595		/* int (boolean) */
#define ATTR_RING_ARROW_LOCATION                    1596		/* int: VAL_LEFT_ANCHOR, VAL_RIGHT_ANCHOR */
#define ATTR_RING_ITEMS_UNIQUE                      1597		/* int (boolean) */
#define ATTR_CASE_SENSITIVE_COMPARE                 1598		/* int (boolean) */
#define ATTR_CELL_SHORTCUT_KEY                      1599		/* int */
#define ATTR_USE_LABEL_TEXT                         1620        /* int (boolean) */
#define ATTR_SIZE_MODE                              1621        /* int: VAL_USE_EXPLICIT_SIZE, ... */
#define ATTR_LABEL_WRAP_MODE                        1622        /* int: VAL_CHAR_WRAP, ... */
#define ATTR_ROW_HEIGHT                             1650        /* int (1-32767) */
#define ATTR_ROW_ACTUAL_HEIGHT                      1651        /* int (not settable) */
#define ATTR_COLUMN_WIDTH                           1700        /* int (1-32767) */
#define ATTR_COLUMN_ACTUAL_WIDTH                    1701        /* int (not settable) */
#define ATTR_COLUMN_VISIBLE                         1702        /* int (boolean) */
#define ATTR_ROW_VISIBLE                            1703        /* int (boolean) */
#define ATTR_DATASOCKET_ENABLED                     1750        /* int (boolean) */
#define ATTR_DS_BIND_PLOT_STYLE                     1770        /* int: VAL_THIN_LINE, ... */
#define ATTR_DS_BIND_POINT_STYLE                    1771        /* int: VAL_EMPTY_SQUARE, ... */
#define ATTR_DS_BIND_LINE_STYLE                     1772        /* int: VAL_SOLID, ... */
#define ATTR_DS_BIND_PLOT_COLOR                     1773        /* int (rgb value) */
#define ATTR_DATASOCKET_SOURCE	                    1791        /* (char *) (not settable) */
#define ATTR_DATASOCKET_SOURCE_LENGTH               1792        /* int (not settable) */
#define ATTR_DATASOCKET_MODE                        1793        /* int (not settable) */
#define ATTR_COLOR_DEPTH                            1800        /* int */
#define ATTR_SYSTEM_MONITOR_HANDLE                  1801        /* intptr_t (HMONITOR) (not settable) */
#define ATTR_NEXT_MONITOR                           1802        /* int */
#define ATTR_POPUP_STYLE                            1850        /* int: VAL_CLASSIC, ... */
#define ATTR_TREE_BGCOLOR                           1900        /* int (rgb value) */
#define ATTR_SHOW_CONNECTION_LINES                  1901        /* int (boolean) */
#define ATTR_SHOW_PLUS_MINUS                        1902        /* int (boolean) */
#define ATTR_SHOW_MARKS                             1903        /* int (boolean) */
#define ATTR_SHOW_IMAGES                            1904        /* int (boolean) */
#define ATTR_MARK_REFLECT                           1905        /* int (boolean) */
#define ATTR_AUTO_EXPAND                            1906        /* int (boolean) */
#define ATTR_AUTO_HSCROLL                           1907        /* int (boolean) */
#define ATTR_FULL_ROW_SELECT                        1908        /* int (boolean) */
#define ATTR_INDENT_OFFSET                          1909        /* int (0-32767 in pixels) */
#define ATTR_RADIO_SIBLING_ALWAYS_MARKED            1910        /* int (boolean) */
#define ATTR_TEXT_CLICK_TOGGLES_MARK                1911        /* int (boolean) */
#define ATTR_HIDE_ACTIVE_ITEM                       1912        /* int (boolean) */
#define ATTR_SELECTION_MODE                         1913        /* int: VAL_SELECTION_MULTIPLE, ... */
#define ATTR_TREE_RUN_STATE                         1914        /* int: VAL_SELECT_STATE, ... */
#define ATTR_ENABLE_DRAG_DROP                       1915        /* int (boolean) */
#define ATTR_ENABLE_DRAG_DROP_COPY                  1924        /* int (boolean) */
#define ATTR_EXPANDED_IMAGE_INDEX                   1916        /* int */
#define ATTR_DISABLE_TREE_TOOLTIPS                  1917        /* int (boolean) */
#define ATTR_DISABLE_TOOLTIPS                       ATTR_DISABLE_TREE_TOOLTIPS
#define ATTR_HIDE_ACTIVE_ITEM_ALWAYS                1918        /* int (boolean) */
#define ATTR_HIDE_HILITE                            1919        /* int (boolean) */
#define ATTR_TREE_EDITABLE_CELLS                    1920        /* int (boolean) */
#define ATTR_COLOR_PICKER_VALUE                     1921        /* int (rgb value) */
#define ATTR_HORIZONTAL_BAR_VALUE                   1922		/* double (0.0-100.0) */
#define ATTR_HORIZONTAL_BAR_COLOR                   1923        /* int (rgb value) */
#define ATTR_COLLAPSED_IMAGE_INDEX                  1931        /* int */
#define ATTR_MARK_TYPE                              1950        /* int: VAL_MARK_NONE, ... */
#define ATTR_MARK_STATE                             1951        /* int: VAL_MARK_OFF, ... */
#define ATTR_SELECTED                               1952        /* int (boolean) */
#define ATTR_COLLAPSED                              1953        /* int (boolean) */
#define ATTR_NO_EDIT_LABEL	                        1954        /* int (boolean) */
#define ATTR_ITEM_HEIGHT                            1955        /* int */
#define ATTR_ITEM_ACTUAL_HEIGHT                     1956        /* int */
#define ATTR_ENABLE_DRAG                            1957        /* int (boolean) */
#define ATTR_ENABLE_DROP                            1958        /* int (boolean) */
#define ATTR_IMAGE_INDEX                            1959        /* int */
#define ATTR_HILITE_ONLY_WHEN_PANEL_ACTIVE          1990        /* int (boolean) */
#define ATTR_TITLEBAR_STYLE                         1991        /* int: VAL_CLASSIC, ... */
#define ATTR_ITEM_BITMAP                            1992		/* int (bitmap id) */
#define ATTR_DOUBLE_CLICK_TOGGLES_COLLAPSE          1993
#define ATTR_TOP_RANGE                              2000		/* int (-32768 - 32767, VAL_AUTO_RANGE, VAL_NO_RANGE_LIMIT) */
#define ATTR_BOTTOM_RANGE                           2001		/* int (-32768 - 32767, VAL_AUTO_RANGE, VAL_NO_RANGE_LIMIT) */
#define ATTR_LEFT_RANGE                             2002		/* int (-32768 - 32767, VAL_AUTO_RANGE, VAL_NO_RANGE_LIMIT) */
#define ATTR_RIGHT_RANGE                            2003		/* int (-32768 - 32767, VAL_AUTO_RANGE, VAL_NO_RANGE_LIMIT) */
#define ATTR_SHOW_CONTENTS_WHILE_DRAGGING           2004        /* int (boolean) */
#define ATTR_SPAN_PANEL                             2005		/* int (boolean) */
#define ATTR_OPERABLE_AS_INDICATOR                  2006		/* int (boolean) */
#define ATTR_TOP_ACTUAL_RANGE                       2007		/* int (not settable) */
#define ATTR_BOTTOM_ACTUAL_RANGE                    2008		/* int (not settable) */
#define ATTR_LEFT_ACTUAL_RANGE                      2009		/* int (not settable) */
#define ATTR_RIGHT_ACTUAL_RANGE                     2010		/* int (not settable) */
#define ATTR_DIGWAVEFORM_AUTOSIZE                   2100		/* int (boolean) */
#define ATTR_DIGWAVEFORM_LINE_LABEL                 2101		/* (char *) */
#define ATTR_DIGWAVEFORM_LINE_LABEL_LENGTH          2102		/* int (not settable) */
#define ATTR_DIGWAVEFORM_BUS_LABEL                  2103		/* (char *) */
#define ATTR_DIGWAVEFORM_BUS_LABEL_LENGTH           2104		/* int (not settable) */
#define ATTR_DIGWAVEFORM_FONT                       2105		/* (char *) */
#define ATTR_DIGWAVEFORM_FONT_NAME_LENGTH           2106		/* int (not settable) */
#define ATTR_DIGWAVEFORM_SHOW_STATE_LABEL           2107		/* int (boolean) */
#define ATTR_DIGWAVEFORM_EXPAND_BUSES               2108		/* int (boolean) */
#define ATTR_DIGWAVEFORM_PLOT_COLOR	                2109		/* int (rgb value) */
#define ATTR_TABS_FIT_MODE                          2200		/* int: VAL_SINGLE_ROW, ... */
#define ATTR_TABS_LOCATION                          2201		/* int: VAL_LEFT_TOP, ... */
#define ATTR_TABS_VISIBLE                           2202		/* int (boolean) */
#define ATTR_TABS_START_OFFSET                      2203        /* int (-32768 - 32767) */
#define ATTR_TABS_END_OFFSET                        2204        /* int (-32768 - 32767) */
#define ATTR_TABS_MAX_TAB_WIDTH                     2205        /* int (-32768 - 32767) */
#define ATTR_NUM_ANNOTATIONS                        2300		/* int (0-10) */
#define ATTR_ANNOTATION_CAPTION                     2301		/* (char *) */
#define ATTR_ANNOTATION_CAPTION_LENGTH              2302		/* int (not settable) */
#define ATTR_ANNOTATION_CAPTION_FONT                2303		/* (char *) */
#define ATTR_ANNOTATION_CAPTION_FONT_NAME_LENGTH    2304	    /* int (not settable) */
#define ATTR_ANNOTATION_CAPTION_COLOR               2305		/* int (rgb value) */
#define ATTR_ANNOTATION_CAPTION_BGCOLOR             2306		/* int (rgb value) */
#define ATTR_ANNOTATION_CAPTION_BOLD                2307        /* int (boolean) */
#define ATTR_ANNOTATION_CAPTION_ITALIC              2308		/* int (boolean) */
#define ATTR_ANNOTATION_CAPTION_UNDERLINE           2309		/* int (boolean) */
#define ATTR_ANNOTATION_CAPTION_STRIKEOUT           2310		/* int (boolean) */
#define ATTR_ANNOTATION_CAPTION_POINT_SIZE          2311		/* int (1-255) */
#define ATTR_ANNOTATION_CAPTION_ANGLE               2312        /* int (-32768 - 32767) */
#define ATTR_ANNOTATION_CAPTION_CHARACTER_SET       2313        /* int: VAL_ANSI_CHARSET, ... */
#define ATTR_ANNOTATION_GLYPH_STYLE                 2320		/* int: VAL_EMPTY_SQUARE, ... */
#define ATTR_ANNOTATION_GLYPH_COLOR                 2321		/* int (rgb value) */
#define ATTR_ANNOTATION_LINE_STYLE                  2325		/* int: VAL_THIN_LINE, ... */
#define ATTR_ANNOTATION_LINE_COLOR                  2326		/* int (rgb value) */
#define ATTR_ANNOTATION_ARROW_STYLE                 2327		/* int: VAL_ARROW, ... */
#define ATTR_ANNOTATION_XVALUE                      2330        /* double */
#define ATTR_ANNOTATION_YVALUE                      2331        /* double */
#define ATTR_ANNOTATION_XOFFSET                     2332        /* int */
#define ATTR_ANNOTATION_YOFFSET                     2333        /* int */
#define ATTR_ANNOTATION_LOCKED                      2340        /* int (boolean) */
#define ATTR_ANNOTATION_XAXIS                       2341		/* int: VAL_BOTTOM_XAXIS or VAL_TOP_XAXIS */
#define ATTR_ANNOTATION_YAXIS                       2342		/* int: VAL_LEFT_YAXIS or VAL_RIGHT_YAXIS */
#define ATTR_ANNOTATION_VISIBLE                     2343        /* int (boolean) */
#define ATTR_ANNOTATION_HIDE_CAPTION                2344        /* int (boolean) */
#define ATTR_ANNOTATION_CAPTION_ALWAYS_IN_VIEW	    2345        /* int (boolean) */
#define ATTR_ANNOTATION_OFFSET_MODE                 2350		/* int: VAL_GLYPH_OFFSET or VAL_TOP_LEFT_OFFSET */
#define ATTR_COLOR_RAMP_WIDTH						2400		/* int (1-255) */
#define ATTR_COLOR_RAMP_INTERPOLATE					2401		/* int (boolean) */
#define ATTR_NUM_COLOR_RAMP_VALUES					2410		/* int (not settable) */
#define ATTR_XLABEL_FONT                            2500        /* (char *) */
#define ATTR_XLABEL_COLOR                           2501        /* int (rgb value) */
#define ATTR_XLABEL_BOLD                            2502        /* int (boolean) */
#define ATTR_XLABEL_ANGLE                           2503        /* int (-32768 - 32767) */
#define ATTR_XLABEL_ITALIC                          2504        /* int (boolean) */
#define ATTR_XLABEL_UNDERLINE                       2505        /* int (boolean) */
#define ATTR_XLABEL_STRIKEOUT                       2506        /* int (boolean) */
#define ATTR_XLABEL_POINT_SIZE                      2507        /* int (1-255) */
#define ATTR_XLABEL_FONT_NAME_LENGTH                2508        /* int (not settable) */
#define ATTR_XLABEL_CHARACTER_SET                   2509        /* int: VAL_ANSI_CHARSET, ... */
#define ATTR_YLABEL_FONT                            2510        /* (char *) */
#define ATTR_YLABEL_COLOR                           2511        /* int (rgb value) */
#define ATTR_YLABEL_BOLD                            2512        /* int (boolean) */
#define ATTR_YLABEL_ANGLE                           2513        /* int (-32768 - 32767) */
#define ATTR_YLABEL_ITALIC                          2514        /* int (boolean) */
#define ATTR_YLABEL_UNDERLINE                       2515        /* int (boolean) */
#define ATTR_YLABEL_STRIKEOUT                       2516        /* int (boolean) */
#define ATTR_YLABEL_POINT_SIZE                      2517        /* int (1-255) */
#define ATTR_YLABEL_FONT_NAME_LENGTH                2518        /* int (not settable) */
#define ATTR_YLABEL_CHARACTER_SET                   2519        /* int: VAL_ANSI_CHARSET, ... */
#define ATTR_NUM_PLOTS                              2520		/* int (not settable) */
#define ATTR_FIRST_PLOT                             2521        /* int (plot Id) (not settable) */
#define ATTR_NEXT_PLOT                              2522		/* int (plot Id) (not settable) */
#define ATTR_PLOT_TYPE                 				2523		/* int: VAL_PLOT_X... (not settable) */
#define ATTR_USE_PROGRESS_BAR_VISUAL_STYLES         2525		/* int (boolean) */
#define ATTR_ROW_OFFSET								2526		/* int */
#define ATTR_COLUMN_OFFSET							2527		/* int */



#endif