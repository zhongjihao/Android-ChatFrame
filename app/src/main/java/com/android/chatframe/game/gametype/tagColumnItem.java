package com.android.chatframe.game.gametype;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-9-2.
 */

//列表项描述结构
public class tagColumnItem {
    char			cbDataDescribe;						//字段类型
    String	        szColumnName;		                //列表名字
    short			wColumnWidth;						//列表宽度
    short 			nFormat;							//水平对齐方式,垂直全部居中

    public tagColumnItem() {

    }

    public char getCbDataDescribe() {
        return cbDataDescribe;
    }

    public void setCbDataDescribe(char cbDataDescribe) {
        this.cbDataDescribe = cbDataDescribe;
    }

    public String getSzColumnName() {
        return szColumnName;
    }

    public void setSzColumnName(String szColumnName) {
        this.szColumnName = szColumnName;
    }

    public short getwColumnWidth() {
        return wColumnWidth;
    }

    public void setwColumnWidth(short wColumnWidth) {
        this.wColumnWidth = wColumnWidth;
    }

    public short getnFormat() {
        return nFormat;
    }

    public void setnFormat(short nFormat) {
        this.nFormat = nFormat;
    }
}
