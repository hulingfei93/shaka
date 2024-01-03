//
// ���ļ����� JavaTM Architecture for XML Binding (JAXB) ����ʵ�� v2.2.7 ���ɵ�
// ����� <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// �����±���Դģʽʱ, �Դ��ļ��������޸Ķ�����ʧ��
// ����ʱ��: 2014.10.11 ʱ�� 11:45:02 PM EDT 
//


package edu.stevens.cs548.clinic.service.dto;

import javax.xml.bind.annotation.XmlEnum;
import javax.xml.bind.annotation.XmlEnumValue;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>SpeicalizationType�� Java �ࡣ
 * 
 * <p>����ģʽƬ��ָ�������ڴ����е�Ԥ�����ݡ�
 * <p>
 * <pre>
 * &lt;simpleType name="SpeicalizationType">
 *   &lt;restriction base="{http://www.w3.org/2001/XMLSchema}string">
 *     &lt;enumeration value="Unknown"/>
 *     &lt;enumeration value="Radiology"/>
 *     &lt;enumeration value="Surgery"/>
 *     &lt;enumeration value="Oncology"/>
 *   &lt;/restriction>
 * &lt;/simpleType>
 * </pre>
 * 
 */
@XmlType(name = "SpeicalizationType")
@XmlEnum
public enum SpeicalizationType {

    @XmlEnumValue("Unknown")
    UNKNOWN("Unknown"),
    @XmlEnumValue("Radiology")
    RADIOLOGY("Radiology"),
    @XmlEnumValue("Surgery")
    SURGERY("Surgery"),
    @XmlEnumValue("Oncology")
    ONCOLOGY("Oncology");
    private final String value;

    SpeicalizationType(String v) {
        value = v;
    }

    public String value() {
        return value;
    }

    public static SpeicalizationType fromValue(String v) {
        for (SpeicalizationType c: SpeicalizationType.values()) {
            if (c.value.equals(v)) {
                return c;
            }
        }
        throw new IllegalArgumentException(v);
    }

}
