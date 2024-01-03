//
// ���ļ����� JavaTM Architecture for XML Binding (JAXB) ����ʵ�� v2.2.7 ���ɵ�
// ����� <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// �����±���Դģʽʱ, �Դ��ļ��������޸Ķ�����ʧ��
// ����ʱ��: 2014.11.23 ʱ�� 07:49:23 PM EST 
//


package edu.stevens.cs548.clinic.service.web.rest.data;

import java.util.Date;
import javax.xml.bind.annotation.adapters.XmlAdapter;

public class Adapter1
    extends XmlAdapter<String, Date>
{


    public Date unmarshal(String value) {
        return (edu.stevens.cs548.clinic.service.web.rest.DateAdapter.parseDate(value));
    }

    public String marshal(Date value) {
        return (edu.stevens.cs548.clinic.service.web.rest.DateAdapter.printDate(value));
    }

}
