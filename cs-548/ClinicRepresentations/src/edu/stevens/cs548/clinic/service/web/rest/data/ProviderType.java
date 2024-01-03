//
// ���ļ����� JavaTM Architecture for XML Binding (JAXB) ����ʵ�� v2.2.7 ���ɵ�
// ����� <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// �����±���Դģʽʱ, �Դ��ļ��������޸Ķ�����ʧ��
// ����ʱ��: 2014.11.23 ʱ�� 07:49:23 PM EST 
//


package edu.stevens.cs548.clinic.service.web.rest.data;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>providerType complex type�� Java �ࡣ
 * 
 * <p>����ģʽƬ��ָ�������ڴ����е�Ԥ�����ݡ�
 * 
 * <pre>
 * &lt;complexType name="providerType">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="NPI" type="{http://www.w3.org/2001/XMLSchema}long"/>
 *         &lt;element name="name" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="specialization" type="{http://cs548.stevens.edu/clinic/service/web/rest/data}SpeicalizationType"/>
 *         &lt;element name="treatments" type="{http://cs548.stevens.edu/clinic/service/web/rest/data/dap}LinkType" maxOccurs="unbounded"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "providerType", propOrder = {
    "npi",
    "name",
    "specialization",
    "treatments"
})
public class ProviderType {

    @XmlElement(name = "NPI")
    protected long npi;
    @XmlElement(required = true)
    protected String name;
    @XmlElement(required = false)
    protected SpeicalizationType specialization;
    @XmlElement(required = true, nillable = true)
    protected List<LinkType> treatments;

    /**
     * ��ȡnpi���Ե�ֵ��
     * 
     */
    public long getNPI() {
        return npi;
    }

    /**
     * ����npi���Ե�ֵ��
     * 
     */
    public void setNPI(long value) {
        this.npi = value;
    }

    /**
     * ��ȡname���Ե�ֵ��
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getName() {
        return name;
    }

    /**
     * ����name���Ե�ֵ��
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setName(String value) {
        this.name = value;
    }

    /**
     * ��ȡspecialization���Ե�ֵ��
     * 
     * @return
     *     possible object is
     *     {@link SpeicalizationType }
     *     
     */
    public SpeicalizationType getSpecialization() {
        return specialization;
    }

    /**
     * ����specialization���Ե�ֵ��
     * 
     * @param value
     *     allowed object is
     *     {@link SpeicalizationType }
     *     
     */
    public void setSpecialization(SpeicalizationType value) {
        this.specialization = value;
    }

    /**
     * Gets the value of the treatments property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the treatments property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getTreatments().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link LinkType }
     * 
     * 
     */
    public List<LinkType> getTreatments() {
        if (treatments == null) {
            treatments = new ArrayList<LinkType>();
        }
        return this.treatments;
    }

}
