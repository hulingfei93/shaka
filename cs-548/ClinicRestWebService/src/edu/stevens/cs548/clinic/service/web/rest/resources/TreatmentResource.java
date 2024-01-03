package edu.stevens.cs548.clinic.service.web.rest.resources;

import javax.annotation.PostConstruct;
import javax.ejb.Stateless;
import javax.enterprise.context.RequestScoped;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.UriInfo;

import edu.stevens.cs548.clinic.domain.ITreatmentDAO;
import edu.stevens.cs548.clinic.domain.ITreatmentDAO.TreatmentException;
import edu.stevens.cs548.clinic.domain.Treatment;
import edu.stevens.cs548.clinic.domain.TreatmentDAO;
import edu.stevens.cs548.clinic.service.web.rest.TreatmentRepresentation;
import edu.stevens.cs548.clinic.service.web.rest.Treatment_PDO_to_Representation;

@Path("/treatment")
@RequestScoped
public class TreatmentResource {

    @Context
    private UriInfo context;
    private ITreatmentDAO treatmentDAO;
    
    public TreatmentResource() {
    }
    
    @PersistenceContext(unitName="ClinicDomain")
    private EntityManager em;
    
    @PostConstruct
    private void initialize() {
        treatmentDAO = new TreatmentDAO(em);
    }
    
    @GET
    @Path("{id}")
    @Produces(MediaType.APPLICATION_XML)
    public TreatmentRepresentation getTreatment(@PathParam("id") String tid) {
       try {
           long id = Long.parseLong(tid);
           Treatment treatment = treatmentDAO.getTreatmentByDbId(id);
           Treatment_PDO_to_Representation visitor = new Treatment_PDO_to_Representation(context);
           treatment.visit(visitor);
           TreatmentRepresentation treatRep = visitor.getRep();
           return treatRep;
       }
       catch(TreatmentException ex) {
           throw new WebApplicationException(Response.Status.NOT_FOUND);
       }
    }
}
