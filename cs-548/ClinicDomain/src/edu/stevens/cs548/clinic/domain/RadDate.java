package edu.stevens.cs548.clinic.domain;

import java.io.Serializable;
import java.util.Date;

import javax.persistence.*;

/**
 * Entity implementation class for Entity: RadDate
 *
 */
@Entity
@Table(name = "radDate")
public class RadDate implements Serializable {
    
    private static final long serialVersionUID = 1L;
    
    @Id
    @GeneratedValue
    private long id;
    
    @Temporal(TemporalType.DATE)
    private Date date;
    
    @ManyToOne
    @JoinColumn(name = "radiologyTreatment_fk", referencedColumnName = "id")
    private RadiologyTreatment radiologyTreatment;
    
    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }
    
    public Date getDate(){
        return date;
    }
    
    public void setDate(Date date) {
        this.date = date;
    }
    
    public RadiologyTreatment getRadiologyTreatment() {
        return radiologyTreatment;
    }
    
    public void setRadiologyTreatment(RadiologyTreatment radiologyTreatment) {
        this.radiologyTreatment = radiologyTreatment;
    }
    
    public RadDate() {
        super();
    }
}
