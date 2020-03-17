import threading

import wx
import pycom
import JDKeyboardUpdate



class mainApp(wx.App):
    def OnInit(self):
        self.SetAppName("断路器校准")
        self.Frame = JDKeyboardUpdate.UpFrame(None)
        #self.Frame = pycom.mainFrame(None)
        self.Frame.Show()
        return True

if __name__ == "__main__":
    #global bserisopen
    #pycom.ti = threading.Thread(target=pycom.ReadData)
    app = mainApp()

    app.MainLoop()
    #if pycom.bserisopen:
    #    pycom.ti.join()
