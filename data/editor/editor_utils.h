/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_utils.h
 * Desc:    Unigine editor
 * Version: 1.28
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *          Valia Vaneeva <fattie@unigine.com>
 *          Anna Ishina <anet@unigne.com>
 *          Andrey Viktorov <unclebob@unigine.com>
 *
 * This file is part of the Unigine engine (http://unigine.com/).
 *
 * Your use and or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the Unigine License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the Unigine License Agreement is available by contacting
 * Unigine Corp. at http://unigine.com/
 */

/******************************************************************************\
*
* Common
*
\******************************************************************************/

/*
 */
void editorWindow(WidgetWindow window) {
	
	window.setMoveable(1);
	#ifndef EDITOR_MINIMAL
		window.setSizeable(1);
	#endif
	window.setTitleable(0);
	window.setBlendable(1);
	window.setFloatable(0);
	window.setSnapDistance(16);
	window.setTextAlign(GUI_ALIGN_LEFT);
	
	gui.addChild(window,GUI_ALIGN_CENTER | GUI_ALIGN_OVERLAP);
	gui.removeChild(window);
	gui.removeChild(window);
}

/*
 */
void editorDialog(WidgetWindow window) {
	
	window.setMoveable(1);
	#ifndef EDITOR_MINMAL
		window.setSizeable(1);
	#endif
	window.setTitleable(0);
	window.setBlendable(0);
	window.setFloatable(0);
	window.setSnapDistance(16);
	window.setTextAlign(GUI_ALIGN_LEFT);
	
	gui.addChild(window,GUI_ALIGN_CENTER | GUI_ALIGN_OVERLAP);
	gui.removeChild(window);
	gui.removeChild(window);
}

/*
 */
void editorEditText(WidgetEditText edittext) {
	edittext.setTokensColor("#define,#ifdef,#ifndef,#elif,#else,#endif,#include",vec4(1.0f,0.0f,1.0f,1.0f));
	edittext.setTokensColor("void,int,float,vec3,vec4,mat4,quat,string",vec4(0.0f,1.0f,0.0f,1.0f));
	edittext.setTokensColor("if,else,for,forloop,foreach,break,continue",vec4(1.0f,1.0f,0.0f,1.0f));
	edittext.setTokensColor("switch,case,new,delete,return,yield,wait",vec4(1.0f,1.0f,0.0f,1.0f));
}

/*
 */
void openExternalEditor(string file,string config = "") {
	if(file == "") return;
	file = savePath(file);
	#ifdef _WIN32
		file = replace(file,"/","\\");
	#endif
	
	if(config != "") {
		string external_editor = getExternalEditor("",config);
		if(is_file(external_editor)) {
			#ifdef _WIN32
				string cmd = format("cmd.exe /c start \"\" \"%s\" \"%s\"", external_editor,file);
			#else
				string cmd = format("%s %s",external_editor,file);
			#endif
			system(cmd,0);
			return;
		}
	}
	
	systemOpen(format("\"%s\"",file));
}

void dialogExternalEditor(string &name,string config) {
	name = getExternalEditor(name,config);
	if(dialogFile(TR("Select External Editor"),"",name,DIALOG_FILE_OPEN)) {
		setExternalEditor(name,config);
	}
}

void setExternalEditor(string name,string config) {
	if(name == "") configSet("external_editor_" + config,"");
	
	name = savePath(name);
	if(is_file(name)) configSet("external_editor_" + config,name);
}

string getExternalEditor(string name,string config) {
	return configGet("external_editor_" + config,name);
}

void systemOpen(string name) {
	#ifdef _WIN32
		system(format("explorer %s",name),0);
	#elif _LINUX
		system(format("xdg-open %s",name),0);
	#elif _MACOS
		system(format("open %s",name),0);
	#endif
}

/*
 */
string editorPath(string name) {
	return relname(engine.getDataPath(),name);
}

string systemPath(string name) {
	string path = savePath(name);
	if(is_file(path)) return path;
	
	path = engine.getDataPath() + engine.filesystem.getFileName(name);
	path = pathname(path) + basename(path);
	if(is_file(path)) return path;
	
	return name;
}

int isAbsolute(string name) {
	#ifdef _WIN32
		if(strlen(name) > 2 && name[1] == ':' && name[2] == '/') return 1;
	#else
		if(strlen(name) > 1 && name[0] == '/') return 1;
	#endif
	
	return 0;
}

string setDiskLetterToLower(string name) {
	#ifdef _WIN32
		return lower(substr(name,0,1)) + substr(name,1,strlen(name) - 1);
	#else
		return name;
	#endif
}

string savePath(string name) {
	if(isAbsolute(name)) return setDiskLetterToLower(name);
	return engine.getDataPath() + name;
}

string systemEditorPath(string name) {
	return editorPath(systemPath(name));
}

/*
 */
int fileCopy(string input,string output) {
	File file = new File();
	if(file.open(output,"wb") == 0) {
		log.error("Can't create \"%s\" file\n",output);
		dialogMessageOk(TR("Error"));
		return 0;
	}
	
	File src = new File();
	if(src.open(input,"rb") == 0) {
		log.error("Can't open \"%s\" file\n",input);
		dialogMessageOk(TR("Error"));
		file.close();
		return 0;
	}
	
	file.writeStream(src,src.getSize());
	src.close();
	file.close();
	
	delete src;
	delete file;
	
	return 1;
}

/*
 */
int fileHash(string name,int bytes = 1024) {
	File file = new File(name,"rb");
	if(file.isOpened() == 0) {
		delete file;
		return -1;
	}
	
	Blob data = new Blob(bytes);
	file.readStream(data,bytes);
	file.close();
	delete file;
	
	int hash[0];
	data.getCRC32(hash);
	delete data;
	
	return hash[0];
}

/*
 */
string fileName(string name) {
	return re_replace(name,"[<>:\"/\\|?*&%]","");
}

/*
 */
string editorFormat(Scalar value,int precision = 3) {
	if(value >= INFINITY) return "inf";
	if(value <= -INFINITY) return "-inf";
	return format("%." + precision + "f",value);
}

/*
 */
string editorFormatMask(int mask) {
	return format("%08x",mask);
}

/*
 */
#ifdef HAS_INTERFACE
InterfaceWindow windows[0];

void updateWindows() {
	int windows_size = engine.interface.getNumWindows();
	if(windows_size == windows.size()) return;
	windows.clear();
	forloop(int i = 0; windows_size) {
		windows.append(engine.interface.getWindow(i));
	}
}

InterfaceWindow getWindow(int i) {
	if(i < 0 || i >= windows.size()) return;
	return windows[i];
}

int getNumWindows() {
	return windows.size();
}

InterfaceWindow getIntersectionWindow() {
	forloop(int i = 0; getNumWindows()) {
		InterfaceWindow window = getWindow(i);
		if(window == NULL) continue;
		if(window.isHidden()) continue;
		if(window.getIntersection(window.getMouseX(), window.getMouseY())) return window;
	}
	
	return NULL;
}

InterfaceWindow findFocusedWindow() {
	
	forloop(int i = 0; getNumWindows()) {
		InterfaceWindow window = getWindow(i);
		if(window.isFocused()) return window;
	}
	
	return NULL;
}

void clearWindows() {
	windows.clear();
}

#endif

/*
 */
int editorIsChild(Widget widget) {
	#ifdef HAS_INTERFACE
		if(gui.isChild(widget)) return 1;
		forloop(int i = 0; getNumWindows()) {
			InterfaceWindow window = getWindow(i);
			if(window == NULL) continue;
			if(window.isHidden() == 0 && Gui(window.getGui()).isChild(widget)) return 1;
		}
	#endif
	return gui.isChild(widget);
}

/*
 */
string setExtension(string name,string ext,string extensions) {
	string current_ext = lower(extension(name));
	if(strlen(current_ext) == 0 || strstr(extensions,current_ext) == -1) name = extension(name,ext);
	return name;
}

string setExtensionImage(string name,string ext) {
	return setExtension(name,ext,".tga.jpg.png.dds.psd.ppm.pgm.hdr.sgi");
}

/*
 */
string getNewName(string name,int non_unique_func) {
	int i = 0;
	string res[0];
	if(re_match(name,"(.*[0-9]+)$")) {
		re_search(name,"([0-9]+)$",res);
		if(res.size() == 1) i = int(res[0]);
		string base = re_replace(name,"([0-9]+)$","");
		do {
			name = format("%s%d",base,i++);
		} while(call(non_unique_func,name));
	} else {
		string base = name;
		do {
			name = format("%s_%d",base,i++);
		} while(call(non_unique_func,name));
	}
	return name;
}

/*
 */
string getNewNodeName(string name) {
	return getNewName(
		name,
		[](string name) {
			return (engine.editor.findNode(name) != -1);
		}
	);
}

/*
 */
string getNewMaterialName(string name) {
	return getNewName(
		name,
		[](string name) {
			return engine.materials.isMaterial(name);
		}
	);
}

string getNewPropertyName(string name) {
	return getNewName(
		name,
		[](string name) {
			return engine.properties.isProperty(name);
		}
	);
}

/*
 */ 
void getArgs(string str,string args[]) {
	args.clear();
	int length = strlen(str);
	int begin = 0;
	forloop(int i = 0; length) {
		if(str[i] == '(') {
			begin = i + 1;
			break;
		}
	}
	for(int i = begin; i < length; i++) {
		int begin = i;
		int braces = 0;
		for(; i < length; i++) {
			int c = str[i];
			if(c == '(') braces++;
			if(c == ')') braces--;
			if(braces == -1) break;
			if(c == ',' && braces == 0) break;
		}
		args.append(substr(str,begin,i - begin));
		if(braces == -1) break;
	}
}

/*
 */
string getWorldPath() {
	
	RegExp world_re = new RegExp(".+\\.world$");
	
	string file_names[0];
	engine.filesystem.getFileNames(file_names);
	foreach(string name; file_names) {
		if(world_re.match(lower(name))) {
			name = replace(name,relname(engine.getAppPath(),engine.getDataPath()),"");
			if(strstr(name,engine.world.getName()) != -1) {
				delete world_re;
				return pathname(name);
			}
		}
	}
	
	dialogMessageOk(TR("Error"),format(TR("Can't find \"%s\" world path"),engine.world.getName()));
	
	delete world_re;
	
	return "";
}

/*
 */
vec3 decomposeRotation(mat4 transform) {
	vec3 r;
	switch(toolsGetRotationOrder()) {
		case 0: r = Unigine::decomposeRotationXYZ(transform); break;
		case 1: r = Unigine::decomposeRotationXZY(transform); break;
		case 2: r = Unigine::decomposeRotationYXZ(transform); break;
		case 3: r = Unigine::decomposeRotationYZX(transform); break;
		case 4: r = Unigine::decomposeRotationZXY(transform); break;
		case 5: r = Unigine::decomposeRotationZYX(transform); break;
	}
	if(r.x < -0.001f) r.x += 360.0f;
	if(r.y < -0.001f) r.y += 360.0f;
	if(r.z < -0.001f) r.z += 360.0f;
	return r;
}

mat4 composeRotation(vec3 r) {
	switch(toolsGetRotationOrder()) {
		case 0: return Unigine::composeRotationXYZ(r);
		case 1: return Unigine::composeRotationXZY(r);
		case 2: return Unigine::composeRotationYXZ(r);
		case 3: return Unigine::composeRotationYZX(r);
		case 4: return Unigine::composeRotationZXY(r);
		case 5: return Unigine::composeRotationZYX(r);
	}
}

/*
 */
void decomposeTransform(Mat4 transform,Vec3 &t,vec3 &r) {
	t = transform.m03m13m23;
	r = decomposeRotation(transform);
}

Mat4 composeTransform(Vec3 t,vec3 r) {
	return translate(Vec3(t)) * composeRotation(r);
}

/*
 */
void decomposeTransform(Mat4 transform,Vec3 &t,vec3 &r,vec3 &s) {
	Mat4 rotation = rotation(transform);
	Mat4 rotate = orthonormalize(rotation);
	mat4 scale = transpose(mat4(rotate)) * rotation;
	t = transform.m03m13m23;
	r = decomposeRotation(rotate);
	s = scale.m00m11m22;
}

Mat4 composeTransform(Vec3 t,vec3 r,vec3 s) {
	return translate(Vec3(t)) * Mat4(composeRotation(r) * scale(s));
}

/******************************************************************************\
*
* Utils
*
\******************************************************************************/

/*
 */
int gui_activity = 0;

void setGuiActivity(int activity) {
	gui_activity = activity;
}

int getGuiActivity() {
	return (gui.getActivity() || gui_activity || nodesIsManipulatorFocused());
}

/*
 */
int clearKeyState(int key) {
	if(key != APP_KEY_ESC && key != APP_KEY_TAB && key != APP_KEY_UP && key != APP_KEY_DOWN) {
		if(gui.getKeyActivity(key)) return 0;
		if(key != APP_KEY_SHIFT && key != APP_KEY_CTRL && key != APP_KEY_ALT) {
			if(engine.controls.isStateKey(key)) return 0;
		}
	}
	return engine.app.clearKeyState(key);
}

/*
 */
void updateManipulator(WidgetManipulator m,float size = 1.0f) {
	Player player = Unigine::getPlayer();
	if(player != NULL) {
		m.setSize(toolsGetSize() * size);
		m.setProjection(player.getProjection());
		m.setModelview(player.getIWorldTransform());
	}
}

void updateAuxManipulator(WidgetManipulator m) {
	updateManipulator(m,0.75f);
}

/*
 */
void setEnabled(int enabled,Widget widgets[]) {
	foreach(Widget w; widgets) {
		w.setEnabled(enabled);
	}
}

/*
 */
void setFontColor(vec4 color,Widget widgets[]) {
	foreach(Widget w; widgets) {
		w.setFontColor(color);
	}
}

/*
 */
void setCallbackEnabled(int callback,int enabled,Widget widgets[]) {
	foreach(Widget w; widgets) {
		w.setCallbackEnabled(callback,enabled);
	}
}

void disableCallbacks(Widget widget) {
	for(int i = GUI_SHOW; i <= GUI_DRAG_DROP; i++) {
		widget.setCallbackEnabled(i,0);
	}
	forloop(int i = 0; widget.getNumChilds()) {
		disableCallbacks(widget.getChild(i));
	}
}

void disableCallbacks(Widget widget,Widget ret[]) {
	ret.append(widget);
	for(int i = GUI_SHOW; i <= GUI_DRAG_DROP; i++) {
		widget.setCallbackEnabled(i,0);
	}
	forloop(int i = 0; widget.getNumChilds()) {
		disableCallbacks(widget.getChild(i),ret);
	}
}

void enableCallbacks(Widget widget) {
	for(int i = GUI_SHOW; i <= GUI_DRAG_DROP; i++) {
		widget.setCallbackEnabled(i,1);
	}
	forloop(int i = 0; widget.getNumChilds()) {
		enableCallbacks(widget.getChild(i));
	}
}

/*
 */
void setChecked(int checked,WidgetCheckBox checkboxes[]) {
	foreach(WidgetCheckBox cb; checkboxes) {
		cb.setChecked(checked);
	}
}

/*
 */
vec4 setSpriteColor(WidgetSprite sprites[],vec4 color) {
	if(color.w == 1.0f) {
		sprites[0].setColor(color);
		sprites[1].setColor(color);
	} else {
		sprites[0].setColor(vec4(color.x,color.y,color.z,1.0f));
		sprites[1].setColor(vec4(color.w,color.w,color.w,1.0f));
	}
	return color;
}

/*
 */
vec3 setEditLineVec3(WidgetEditLine editlines[],vec3 value) {
	editlines[0].setText(editorFormat(value.x));
	editlines[1].setText(editorFormat(value.y));
	editlines[2].setText(editorFormat(value.z));
	return value;
}

vec3 getEditLineVec3(WidgetEditLine editlines[]) {
	vec3 ret;
	ret.x = float(editlines[0].getText());
	ret.y = float(editlines[1].getText());
	ret.z = float(editlines[2].getText());
	return ret;
}

/*
 */
Vec3 setEditLineDVec3(WidgetEditLine editlines[],Vec3 value) {
	editlines[0].setText(editorFormat(value.x));
	editlines[1].setText(editorFormat(value.y));
	editlines[2].setText(editorFormat(value.z));
	return value;
}

Vec3 getEditLineDVec3(WidgetEditLine editlines[]) {
	Vec3 ret;
	ret.x = Scalar(editlines[0].getText());
	ret.y = Scalar(editlines[1].getText());
	ret.z = Scalar(editlines[2].getText());
	return ret;
}

/*
 */
vec4 setEditLineVec4(WidgetEditLine editlines[],vec4 value) {
	editlines[0].setText(editorFormat(value.x));
	editlines[1].setText(editorFormat(value.y));
	editlines[2].setText(editorFormat(value.z));
	editlines[3].setText(editorFormat(value.w));
	return value;
}

vec4 getEditLineVec4(WidgetEditLine editlines[]) {
	vec4 ret;
	ret.x = float(editlines[0].getText());
	ret.y = float(editlines[1].getText());
	ret.z = float(editlines[2].getText());
	ret.w = float(editlines[3].getText());
	return ret;
}

/*
 */
vec3 setEditLineSameVec3(WidgetEditLine editlines[],vec3 value) {
	editlines[0].setText(editorFormat(value.x));
	if(editlines[1].isFocused() == 0 && value.y == value.x) editlines[1].setText("same");
	else editlines[1].setText(editorFormat(value.y));
	if(editlines[2].isFocused() == 0 && value.z == value.x) editlines[2].setText("same");
	else editlines[2].setText(editorFormat(value.z));
	return value;
}

vec3 getEditLineSameVec3(WidgetEditLine editlines[]) {
	vec3 ret;
	ret.x = float(editlines[0].getText());
	if(lower(editlines[1].getText()) == "same") ret.y = ret.x;
	else ret.y = float(editlines[1].getText());
	if(lower(editlines[2].getText()) == "same") ret.z = ret.x;
	else ret.z = float(editlines[2].getText());
	return ret;
}

/*
 */
vec4 setEditLineSameVec4(WidgetEditLine editlines[],vec4 value) {
	editlines[0].setText(editorFormat(value.x));
	if(editlines[1].isFocused() == 0 && value.y == value.x) editlines[1].setText("same");
	else editlines[1].setText(editorFormat(value.y));
	if(editlines[2].isFocused() == 0 && value.z == value.x) editlines[2].setText("same");
	else editlines[2].setText(editorFormat(value.z));
	if(editlines[3].isFocused() == 0 && value.w == value.x) editlines[3].setText("same");
	else editlines[3].setText(editorFormat(value.w));
	return value;
}

vec4 getEditLineSameVec4(WidgetEditLine editlines[]) {
	vec4 ret;
	ret.x = float(editlines[0].getText());
	if(lower(editlines[1].getText()) == "same") ret.y = ret.x;
	else ret.y = float(editlines[1].getText());
	if(lower(editlines[2].getText()) == "same") ret.z = ret.x;
	else ret.z = float(editlines[2].getText());
	if(lower(editlines[3].getText()) == "same") ret.w = ret.x;
	else ret.w = float(editlines[3].getText());
	return ret;
}

/*
 */
void appendToEditLine(Widget focus,float value,WidgetEditLine editlines[]) {
	foreach(WidgetEditLine el; editlines) {
		if(el == focus) {
			el.setText(editorFormat(float(el.getText()) + value));
			el.runCallback(GUI_PRESSED);
			break;
		}
	}
}

/*
 */
void setEditLineSliderValue(WidgetEditLine editline,WidgetSlider slider,int value) {
	slider.setCallbackEnabled(GUI_CHANGED,0);
	slider.setValue(value);
	slider.setCallbackEnabled(GUI_CHANGED,1);
	editline.setText(string(value));
}

void setEditLineSliderValue(WidgetEditLine editline,WidgetSlider slider,float value,float scale) {
	slider.setCallbackEnabled(GUI_CHANGED,0);
	value *= scale;
	slider.setValue(value);
	slider.setCallbackEnabled(GUI_CHANGED,1);
	value /= scale;
	editline.setText(editorFormat(value));
}

void setEditLineSliderExpandedValue(WidgetEditLine editline,WidgetSlider slider,float value,float scale) {
	slider.setCallbackEnabled(GUI_CHANGED,0);
	value *= scale;
	if(slider.getMaxValue() < value) slider.setMaxValue(min(value,1000000.0f));
	if(slider.getMinValue() > value) slider.setMinValue(max(value,-1000000.0f));
	slider.setValue(value);
	slider.setCallbackEnabled(GUI_CHANGED,1);
	value /= scale;
	editline.setText(editorFormat(value));
}

/*
 */
int selectTreeBoxByName(WidgetTreeBox treebox,WidgetEditLine editline,int next = 0) {
	
	int has_selection = 0;
	int selection_changed = 0;
	string name = lower(editline.getText());
	
	int first_item = -1;
	
	int items[0];
	treebox.getItems(items);
	foreach(int item; items) {
		string item_text = lower(treebox.getItemText(item));
		int selected = (name != "") && (strstr(item_text,name) >= 0);
		has_selection |= selected;
		
		if(selected) {
			int parent = treebox.getItemParent(item);
			while(parent != -1) {
				treebox.setItemFolded(parent,0);
				parent = treebox.getItemParent(parent);
			}
		}
		
		if(treebox.isMultiSelection()) {
			if(selected) selection_changed = 1;
			treebox.setItemSelected(item,selected);
		}
		else if(selected) {
			if(next && treebox.getCurrentItem() >= item) {
				if(first_item == -1) first_item = item;
				continue;
			}
			if(treebox.getCurrentItem() != item) {
				treebox.setCurrentItem(item);
				treebox.showItem(item);
				selection_changed = 1;
			}
			break;
		}
	}
	
	if(first_item != -1 && selection_changed == 0) {
		treebox.setCurrentItem(first_item);
		treebox.showItem(first_item);
		selection_changed = 1;
	}
	
	if(has_selection || strlen(name) == 0) editline.setFontColor(vec4_zero);
	else editline.setFontColor(color_red);
	
	return selection_changed;
}

/*
 */
void saveTreeBoxFolded(WidgetTreeBox treebox,int folded[]) {
	folded.clear();
	int items[0];
	treebox.getItems(items);
	foreach(int item; items) {
		folded.append(treebox.getItemText(item),treebox.isItemFolded(item));
	}
}

void restoreTreeBoxFolded(WidgetTreeBox treebox,int folded[]) {
	int items[0];
	treebox.getItems(items);
	foreach(int item; items) {
		string name = treebox.getItemText(item);
		if(folded.check(name)) treebox.setItemFolded(item,folded[name]);
	}
}

/******************************************************************************\
*
* Dir
*
\******************************************************************************/

/*
 */
void dirTraverse(string path,int dir_func,int file_func) {
	
	if(call(dir_func,path) == 0) return;
	
	// read dir contents
	Dir dir = new Dir();
	if(dir.open(path) == 0) {
		delete dir;
		return;
	}
	
	// process files
	forloop(int i = 0; dir.getNumFiles()) {
		call(file_func,dir.getFileName(i));
	}
	
	// process subdirs
	forloop(int i = 0; dir.getNumDirs()) {
		dirTraverse(dir.getDirName(i),dir_func,file_func);
	}
	
	dir.close();
	delete dir;
}

/******************************************************************************\
*
* Xml
*
\******************************************************************************/

/*
 */
int xmlSetData(Xml xml,string name,int value) {
	Xml x = xml.addChild(name);
	if(is_int(value)) x.setIntData(value);
	else if(is_float(value)) x.setFloatData(value);
	else if(is_vec3(value)) x.setVec3Data(value);
	else if(is_vec4(value)) x.setVec4Data(value);
	else if(is_mat4(value)) x.setMat4Data(value);
	else if(is_quat(value)) x.setQuatData(value);
	else if(is_string(value)) x.setData(value);
	else throw("xmlSetData(): unknown value type\n");
}

int xmlGetData(Xml xml,string name,int value) {
	if(xml.isChild(name) == 0) return value;
	Xml x = xml.getChild(name);
	if(is_int(value)) return x.getIntData();
	if(is_float(value)) return x.getFloatData();
	if(is_vec3(value)) return x.getVec3Data();
	if(is_vec4(value)) return x.getVec4Data();
	if(is_mat4(value)) return x.getMat4Data();
	if(is_quat(value)) return x.getQuatData();
	if(is_string(value)) return x.getData();
	throw("xmlGetData(): unknown value type\n");
}

/*
 */
void foreach_xml_node(Xml node,int func) {
	call(func,node);
	
	forloop(int i = 0; node.getNumChilds()) {
		foreach_xml_node(node.getChild(i),func);
	}
}

/******************************************************************************\
*
* Image
*
\******************************************************************************/

/*
 */
int createDummyTextureArray(string title,string &name) {
	name = setExtensionImage(name,"dds");
	
	if(is_file(savePath(name))) return 1;
	
	if(dialogMessageYesNo(TR("Confirm"),TR("Texture array is not created. Would you like to create one?")) == 0) return 0;
	
	if(dialogFile(title,".dds",name,DIALOG_FILE_SAVE) == 0) return 0;
	
	// create dummy array
	Image array = class_manage(new Image());
	array.create2DArray(256,256,1,IMAGE_FORMAT_RGBA8);
	
	if(array.save(name) == 0) {
		dialogMessageOk(TR("Error"));
		return 0;
	}
	
	return 1;
}

/*
 */
void applyGamma(Image image,float gamma = 2.2f) {
	float igamma = 1.0f / gamma;
	forloop(int y = 0; image.getHeight()) {
		forloop(int x = 0; image.getWidth()) {
			vec4 linear_color = image.get2D(x,y);
			vec4 srgb_color = vec4(pow(linear_color.r,igamma),pow(linear_color.g,igamma),pow(linear_color.b,igamma),linear_color.a);
			image.set2D(x,y,srgb_color);
		}
	}
}

/******************************************************************************\
*
* Window
*
\******************************************************************************/

/*
 */
int isPathValid(string path) {
	if(is_file(path)) return true;
	
	File file = new ::File();
	if(file.open(path,"wb")) {
		file.close();
		delete file;
		remove(path);
		return true;
	}
	
	delete file;
	return false;
}

/*
 */
void windowInit(Unigine::Widgets::Window window,string key) {
	int window_show = configGet(key + "_show",1);
	if(window_show) windowShow(window,key);
}

void windowShutdown(Unigine::Widgets::Window window,string key) {
	if(Unigine::Widgets::isChild(window)) {
		if(window.isSizeable()) {
			configSet(key + "_width",window.getWidth());
			configSet(key + "_height",window.getHeight());
		}
		configSet(key + "_position_x",window.getPositionX());
		configSet(key + "_position_y",window.getPositionY());
	}
}

void windowToggle(Unigine::Widgets::Window window,string key) {
	if(Unigine::Widgets::isChild(window)) windowHide(window,key);
	else windowShow(window,key);
}

void windowShow(Unigine::Widgets::Window window,string key) {
	Unigine::Widgets::addChild(window,GUI_ALIGN_OVERLAP | GUI_ALIGN_CENTER);
	
	if(window.isSizeable()) {
		window.setWidth(configGet(key + "_width",window.getWidth()));
		window.setHeight(configGet(key + "_height",window.getHeight()));
		#ifdef HAS_INTERFACE
			window.getInterface().setWidth(window.getWidth());
			window.getInterface().setHeight(window.getHeight());
		#endif
	}
	int x = configGet(key + "_position_x",window.getPositionX());
	int y = configGet(key + "_position_y",window.getPositionY());
	
	window.setPosition(x,y);
	window.setFocus();
	#ifdef HAS_INTERFACE
		window.getInterface().setFocus();
	#endif
	
	configSet(key + "_show",1);
}

void windowHide(Unigine::Widgets::Window window,string key) {
	if(window.isSizeable()) {
		configSet(key + "_width",window.getWidth());
		configSet(key + "_height",window.getHeight());
	}
	configSet(key + "_position_x",window.getPositionX());
	configSet(key + "_position_y",window.getPositionY());
	
	Unigine::Widgets::removeChild(window);
	
	configSet(key + "_show",0);
}

/******************************************************************************\
*
* Worlds
*
\******************************************************************************/

void getAllWorlds(string ret[]) {
	
	int check_hidden(string path) {
		string parts[0];
		strsplit(path,"/",parts);
		
		path = engine.getDataPath();
		foreach(string p; parts) {
			if(strlen(p) && p[0] == '.') return false;
			path += "/" + p;
			if(is_hidden(path)) return false;
		}
		
		return true;
	}
	
	RegExp world_re = new RegExp(".+\\.world$");
	
	string file_names[0];
	engine.filesystem.getFileNames(file_names);
	
	foreach(string name; file_names) {
		if(world_re.match(lower(name))) {
			
			name = replace(name,relname(engine.getAppPath(),engine.getDataPath()),"");
			name = replace(name,engine.getDataPath(),"");
			name = replace(name,engine.getAppPath(),"");
			
			#ifdef _LINUX || _MACOS
				if(name[0] == '/' && is_file(relname(engine.getDataPath(),name)) == false) continue;
				else if(name[0] != '/' && is_file(absname(engine.getDataPath(),name)) == false) continue;
			#else
				if(is_file(absname(engine.getDataPath(),name)) == false) continue;
			#endif
			
			if(!check_hidden(name)) continue;
			
			name = extension(name,NULL);
			
			if(name == "editor/resources/template") continue;
			if(name == "editor/plugins/game_framework_plugin/editor/templates/level/template") continue;
			if(ret.find(name) != -1) continue;
			
			ret.append(name);
		}
	}
	
	delete world_re;
}

string getCurrentWorldName() {
	string name = engine.filesystem.getFileName(engine.world.getName() + ".cpp");
	name = replace(name,relname(engine.getAppPath(),engine.getDataPath()),"");
	name = replace(name,engine.getDataPath(),"");
	name = replace(name,engine.getAppPath(),"");
	name = extension(name,NULL);
	return name;
}

/******************************************************************************\
*
* Terrain
*
\******************************************************************************/

/*
 */
void optimizeTerrainMask(Image &mask_array,ObjectTerrain terrain) {
	dialogProgressBegin(TR("Optimize mask textures..."));
	Image image_delete = new Image();
	int size = mask_array.getHeight();
	image_delete.create2D(size,size,IMAGE_FORMAT_R8);
	
	int num_materials = terrain.getNumMaterials();
	forloop(int i = 0; num_materials - 1) {
		if(!terrain.getMaterialOverlap(i)) continue;
		image_delete.set(0,0);
		
		forloop(int j = i + 1; num_materials) {
			if(!terrain.getMaterialOverlap(i)) continue;
			image_delete.blend(mask_array,0,0,0,j * size,size,size,1.0f,0);
		}
		
		image_delete.blur(2);
		forloop(int y = 0; size) {
			forloop(int x = 0; size) {
				vec4 p_m = image_delete.get2D(x,y);
				if(p_m.r >= 1.0f) image_delete.set2D(x,y,vec4_one);
				else image_delete.set2D(x,y,vec4_zero);
			}
		}
		mask_array.blend(image_delete,0,i * size,0,0,size,size,-1.0f,0);
		
		int progress = i * 100 / num_materials;
		dialogProgressUpdate(progress);
	}
	delete image_delete;
	
	dialogProgressEnd();
}

/*
 */
string getNewMaterialNameForTerrain(ObjectTerrain terrain) {
	string current_names[];
	current_names.clear();
	
	forloop(int i = 0; terrain.getNumMaterials()) current_names.append(terrain.getMaterialName(i));
	
	int counter = terrain.getNumMaterials();
	while(1) {
		string candidate = format("Material %d",counter);
		
		if(current_names.check(candidate) == 0) return candidate;
		counter++;
	}
	return "";
}

/*
 */
string node_type_names[] = (
	
	"NodeDummy",
	"NodeLayer",
	"NodePivot",
	"NodeTrigger",
	"NodeReference",
	"NodeExtern",
	
	"WorldLayer",
	"WorldSector",
	"WorldPortal",
	"WorldTrigger",
	"WorldCluster",
	"WorldClutter",
	"WorldSwitcher",
	"WorldOccluder",
	"WorldOccluderMesh",
	"WorldOccluderTerrain",
	"WorldTransformPath",
	"WorldTransformBone",
	"WorldExpression",
	"WorldExtern",
	
	"FieldSpacer",
	"FieldAnimation",
	
	"LightProb",
	"LightSpot",
	"LightOmni",
	"LightProj",
	"LightWorld",
	
	"DecalObjectOmni",
	"DecalObjectProj",
	"DecalObjectOrtho",
	"DecalTerrainProj",
	"DecalTerrainOrtho",
	"DecalDeferredProj",
	"DecalDeferredOrtho",
	"DecalDeferredMesh",
	
	"ObjectDummy",
	"ObjectDynamic",
	"ObjectMeshStatic",
	"ObjectMeshCluster",
	"ObjectMeshClutter",
	"ObjectMeshSkinned",
	"ObjectMeshDynamic",
	"ObjectTerrain",
	"ObjectGrass",
	"ObjectParticles",
	"ObjectBillboard",
	"ObjectBillboards",
	"ObjectVolumeBox",
	"ObjectVolumeSphere",
	"ObjectVolumeOmni",
	"ObjectVolumeProj",
	"ObjectGui",
	"ObjectGuiMesh",
	"ObjectWater",
	"ObjectWaterMesh",
	"ObjectSky",
	"ObjectExtern",
	
	"PlayerDummy",
	"PlayerSpectator",
	"PlayerPersecutor",
	"PlayerActor",
	
	"PhysicalWind",
	"PhysicalForce",
	"PhysicalNoise",
	"PhysicalWater",
	"PhysicalTrigger",
	
	"NavigationSector",
	"NavigationMesh",
	
	"ObstacleBox",
	"ObstacleSphere",
	"ObstacleCapsule",
	
	"SoundSource",
	"SoundReverb",
);

int getNodeType(string name) {
	return node_type_names.find(name);
}

string getNodeTypeName(int type) {
	return node_type_names[type];
}

int upperSymbol(int symbol) {
	if(symbol >= 'a' && symbol <= 'z') symbol -= ('a' - 'A');
	return symbol;
}

int lowerSymbol(int symbol) {
	if(symbol >= 'A' && symbol <= 'Z') symbol += ('a' - 'A');
	return symbol;
}

//TODO: drop this and make autolayout
void set_widget_position(Widget w) {
	Editor::Widgets::WidgetViewportLayout layout = viewportsLayoutGet();
	
	int left = gui.getWidth() - w.getWidth();
	int top = gui.getHeight() - w.getHeight();
	int right = gui.getWidth();
	int bottom = gui.getHeight();
	
	Editor::Widgets::WidgetViewportLayoutPanel panels[0];
	layout.getIntersectedPanels(left,top,right,bottom,panels);
	
	while(panels.size() != 0) {
		
		int min_y = panels[0].panel_box.getPositionY();
		
		for(int i = 1; i < panels.size(); i++) {
			int val = panels[i].panel_box.getPositionY();
			if(min_y > val) min_y = val;
		}
		
		bottom = min_y;
		top = min_y - w.getHeight();
		
		layout.getIntersectedPanels(left,top,right,bottom,panels);
	}
	
	w.setPosition(left,top);
}
