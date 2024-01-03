//
// 此文件是由 JavaTM Architecture for XML Binding (JAXB) 引用实现 v2.2.7 生成的
// 请访问 <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// 在重新编译源模式时, 对此文件的所有修改都将丢失。
// 生成时间: 2014.11.23 时间 07:49:23 PM EST 
//


package edu.stevens.cs548.clinic.service.web.rest.data;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>providerType complex type的 Java 类。
 * 
 * <p>以下模式片段指定包含在此类中的预期内容。
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
     * 获取npi属性的值。
     * 
     */
    public long getNPI() {
        return npi;
    }

    /**
     * 设置npi属性的值。
     * 
     */
    public void setNPI(long value) {
        this.npi = value;
    }

    /**
     * 获取name属性的值。
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
     * 设置name属性的值。
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
     * 获取specialization属性的值。
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
     * 设置specialization属性的值。
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
