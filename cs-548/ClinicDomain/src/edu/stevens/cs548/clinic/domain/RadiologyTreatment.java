package edu.stevens.cs548.clinic.domain;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import javax.persistence.*;

import static javax.persistence.CascadeType.REMOVE;

/**
 * Entity implementation class for Entity: RadiologyTreatment
 *
 */
@Entity
@Table(name = "radiologyTreatment")
@DiscriminatorValue("R")
public class RadiologyTreatment extends Treatment implements Serializable {

    private static final long serialVersionUID = 1L;

    @OneToMany(cascade = REMOVE, mappedBy = "radiologyTreatment")
    @OrderBy
    private List<RadDate> dates;

    public List<RadDate> getDates() {
        return dates;
    }

    public void setDates(List<RadDate> dates) {
        this.dates = dates;
    }
    
    public void addDate(RadDate date) {
        this.getDates().add(date);
    }
    
    @Override
    public void visit(ITreatmentVisitor visitor) {
        visitor.visitRadiologyTreatment(this.getId(), this.getPatient().getId(), this.getProvider().getNPI(), this.getDiagnosis(), this.dates);
    }
    
    public RadiologyTreatment() {
        super();
        setTreatmentType("R");
        dates = new ArrayList<RadDate>();
    }
}
