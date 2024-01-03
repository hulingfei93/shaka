package edu.stevens.cs548.clinic.service.web.rest.resources.test;

import javax.annotation.PostConstruct;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import edu.stevens.cs548.clinic.domain.ITreatmentDAO;
import edu.stevens.cs548.clinic.domain.ITreatmentDAO.TreatmentException;
import edu.stevens.cs548.clinic.domain.Treatment;
import edu.stevens.cs548.clinic.domain.TreatmentDAO;
import edu.stevens.cs548.clinic.service.web.rest.TreatmentRepresentation;
import edu.stevens.cs548.clinic.service.web.rest.Treatment_PDO_to_Representation_Test;

@Stateless(name="TreatmentResourceTest")
public class TreatmentResourceTest {
    
    private ITreatmentDAO treatmentDAO;
    
    public TreatmentResourceTest() {
    }
    
    @PersistenceContext(unitName="ClinicDomain")
    private EntityManager em;
    
    @PostConstruct
    private void initialize() {
        treatmentDAO = new TreatmentDAO(em);
    }
    
    public TreatmentRepresentation getTreatment(String tid) {
       try {
           long id = Long.parseLong(tid);
           Treatment treatment = treatmentDAO.getTreatmentByDbId(id);
           Treatment_PDO_to_Representation_Test visitor = new Treatment_PDO_to_Representation_Test();
           treatment.visit(visitor);
           TreatmentRepresentation treatRep = visitor.getRep();
           return treatRep;
       }
       catch(TreatmentException ex) {
           throw new WebApplicationException(Response.Status.NOT_FOUND);
       }
    }
}
