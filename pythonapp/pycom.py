import binascii

import serial
import wx
import sys
import threading, time

textPortNum = None
STRGLO = ""  # 读取的数据
bserisopen = False  # 读取标志位
ser = None
utext = None
itext = None
ptext = None
aptext = None
potext = None
protext = None
idtext = None
is_exit = False
txt = None



RxData = []
def closeserial(ser):
    global bserisopen
    #if bserisopen:
    #    threading.Thread._Thread__stop(ti)
    if (ser!= None and ser.is_open):
        is_exit = False
        bserisopen = False
        ser.close()



def ReadData():
    global STRGLO
    global bserisopen
    while True:
        if bserisopen and ser.in_waiting:
            count = 0
            while count < ser.in_waiting:
                #RxData.append(hex(ser.read(1)))
                #STRGLO = hex(ser.read(1))
                #STRGLO = hex(int.from_bytes(ser.read(1)),byteorder='big')
                STRGLO = binascii.b2a_hex(ser.read(1)).decode() + ' '
                #STRGLO = str(ser.read(1), encoding="gbk")
                count = count+1
                protext.AppendText(STRGLO)
        print("thead read com\r\n")
        if bserisopen == False:
            print("thead end\r\n")
            break



def openserial():
    global ser
    global ti
    global bserisopen
    try:
        portx = "com" + textPortNum.GetValue()
        print(portx)
        bps = 4800
        timex = 5
        ser = serial.Serial(portx, bps, timeout=timex)
        if (ser.is_open):
            bserisopen = True
            ti = threading.Thread(target=ReadData)
            ti.start()
    except Exception as e:
        print("----异常：", e)


def WritePort(ser, text):
    if (bserisopen == False):
        openserial()
    if (bserisopen):
        result = ser.write(text.encode("gbk"))
        return result


def u_adjust(event):
    global ser
    if (bserisopen == False):
        openserial()
    cmd = "AUG" + utext.GetValue()
    result = ser.write(cmd.encode("gbk"))
    return result


def i_adjust(event):
    global ser
    if (bserisopen == False):
        openserial()
    cmd = "AIA" + itext.GetValue()
    result = ser.write(cmd.encode("gbk"))
    return result


def p_adjust(event):
    global ser
    if (bserisopen == False):
        openserial()
    cmd = "APA" + ptext.GetValue()
    result = ser.write(cmd.encode("gbk"))
    return result


def ap_adjust(event):
    global ser
    if (bserisopen == False):
        openserial()
    cmd = "AAP" + aptext.GetValue()
    result = ser.write(cmd.encode("gbk"))
    return result


def po_adjust(event):
    global ser
    if (bserisopen == False):
        openserial()
    cmd = "APO" + potext.GetValue()
    result = ser.write(cmd.encode("gbk"))
    return result


def set_id(event):
    global ser
    if (bserisopen == False):
        openserial()
    cmd = "ID:" + idtext.GetValue()
    result = ser.write(cmd.encode("gbk"))
    return result


def open(event):
    global ser
    if (bserisopen == False):
        openserial()
    if (bserisopen):
        cmd = "ID:" + idtext.GetValue()
        result = ser.write(cmd.encode("gbk"))
        return result


def close(event):
    global ser
    if (bserisopen == False):
        openserial()
    if (bserisopen):
        cmd = "ID:" + idtext.GetValue()
        result = ser.write(cmd.encode("gbk"))
        return result


def lookover(event):
    global ser
    if (bserisopen == False):
        openserial()
    cmd = "ID:" + idtext.GetValue()
    result = ser.write(cmd.encode("gbk"))
    return result


def onChange(event):
    global txt
    txt.AppendText("qq");
    return result


# frame = wx.Frame(None,title = "断路器校准",pos = (500,200),size = (500,400))
#
# open_button = wx.Button(frame,label = "打开",pos = (100,5),size = (50,24))
# open_button.Bind(wx.EVT_BUTTON,openserial)    # 绑定打开文件事件到open_button按钮上

class mainFrame(wx.Frame):
    def __init__(self, parent):
        global textPortNum
        global utext
        global itext
        global ptext
        global aptext
        global potext
        global protext
        global idtext
        global txt
        '''构造函数'''
        wx.Frame.__init__(self, parent, -1, "断路器校准")
        self.SetBackgroundColour(wx.Colour(224, 224, 224))
        self.SetSize((800, 600))
        self.Center()
        self.SetWindowStyle(wx.DEFAULT_FRAME_STYLE)

        self.Bind(wx.EVT_CLOSE, self.OnClose)

        wx.StaticText(self, -1, "串口号", (5, 5))
        textPortNum = wx.TextCtrl(self, pos=(50, 5), size=(80, 24))
        # open_button = wx.Button(self, label="打开", pos=(200, 5), size=(50, 24))
        # open_button.Bind(wx.EVT_BUTTON, openserial)

        u_button = wx.Button(self, label="电压校准,单位V", pos=(100, 40), size=(150, 24))
        u_button.Bind(wx.EVT_BUTTON, u_adjust)
        utext = wx.TextCtrl(self, pos=(5, 40), size=(80, 24))

        self.Bind(wx.EVT_TEXT, onChange, utext)

        u_button = wx.Button(self, label="电流校准,单位MA", pos=(100, 80), size=(150, 24))
        u_button.Bind(wx.EVT_BUTTON, i_adjust)
        itext = wx.TextCtrl(self, pos=(5, 80), size=(80, 24))

        u_button = wx.Button(self, label="功率校准,单位W", pos=(100, 120), size=(150, 24))
        u_button.Bind(wx.EVT_BUTTON, p_adjust)
        ptext = wx.TextCtrl(self, pos=(5, 120), size=(80, 24))

        u_button = wx.Button(self, label="角差校准,单位W", pos=(100, 160), size=(150, 24))
        u_button.Bind(wx.EVT_BUTTON, ap_adjust)
        aptext = wx.TextCtrl(self, pos=(5, 160), size=(80, 24))

        u_button = wx.Button(self, label="功率失调校准,单位W", pos=(100, 200), size=(150, 24))
        u_button.Bind(wx.EVT_BUTTON, po_adjust)
        potext = wx.TextCtrl(self, pos=(5, 200), size=(80, 24))

        u_button = wx.Button(self, label="设置ID", pos=(100, 240), size=(150, 24))
        u_button.Bind(wx.EVT_BUTTON, set_id)
        idtext = wx.TextCtrl(self, pos=(5, 240), size=(80, 24))

        protext = wx.TextCtrl(self, pos=(260, 5), size=(500, 500), style=wx.TE_MULTILINE)

        open_button = wx.Button(self, label="打开", pos=(0, 280), size=(80, 30))
        open_button.Bind(wx.EVT_BUTTON, open)

        close_button = wx.Button(self, label="关闭", pos=(90, 280), size=(80, 30))
        close_button.Bind(wx.EVT_BUTTON, close)

        look_button = wx.Button(self, label="查询", pos=(180, 280), size=(80, 30))
        look_button.Bind(wx.EVT_BUTTON, lookover)

        txt = wx.StaticText(self, -1, 'Hello World', pos=(2, 320), size=(200, 30))

    def OnClose(self, evt):
        dlg = wx.MessageDialog(self, u'确定要关闭本窗口？', u'操作提示', wx.YES_NO | wx.ICON_QUESTION)
        if (dlg.ShowModal() == wx.ID_YES):
            closeserial(ser)
            self.Destroy()
