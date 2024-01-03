package edu.stevens.cs548.clinic.service.web.rest.resources;

import java.net.URI;
import java.util.Date;

import javax.ws.rs.Consumes;
import javax.ws.rs.FormParam;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;
import javax.ws.rs.core.UriBuilder;
import javax.ws.rs.core.UriInfo;
import javax.ws.rs.core.Response.Status;
import javax.ws.rs.PathParam;
import javax.ws.rs.QueryParam;
import javax.ejb.EJB;
import javax.ejb.Stateless;
import javax.xml.bind.DatatypeConverter;
import javax.enterprise.context.RequestScoped;
import javax.inject.Inject;
import javax.inject.Named;

import edu.stevens.cs548.clinic.service.web.rest.PatientRepresentation;
import edu.stevens.cs548.clinic.service.dto.PatientDTO;
import edu.stevens.cs548.clinic.service.ejb.IPatientService.PatientNotFoundException;
import edu.stevens.cs548.clinic.service.ejb.IPatientService.PatientServiceException;
import edu.stevens.cs548.clinic.service.ejb.IPatientServiceRemote;

@Path("/patient")
@RequestScoped
public class PatientResource {

    @Context
    private UriInfo context;

    /**
     * Default constructor. 
     */
    public PatientResource() {
    }
    
    @EJB
    private IPatientServiceRemote patientService;

    /**
     * Retrieves representation of an instance of PatientResource
     * @return an instance of String
     */
    @GET
    @Path("{id}")
    @Produces(MediaType.APPLICATION_XML)
    public PatientRepresentation getPatient(@PathParam("id") String id) {
       try {
           long key = Long.parseLong(id);
           PatientDTO patientDTO = patientService.getPatientByDbId(key);
           PatientRepresentation patientRep = new PatientRepresentation(patientDTO, context);
           return patientRep;
       }
       catch(PatientServiceException ex) {
           throw new WebApplicationException(Response.Status.NOT_FOUND);
       }
    }
    
    @GET
    @Path("patientId")
    @Produces(MediaType.APPLICATION_XML)
    public PatientRepresentation getPatientByPatientId(@QueryParam("id") String patientId) {
       try {
           long pid = Long.parseLong(patientId);
           PatientDTO patientDTO = patientService.getPatientByPatientId(pid);
           PatientRepresentation patientRep = new PatientRepresentation(patientDTO, context);
           return patientRep;
       }
       catch(PatientNotFoundException ex) {
           throw new WebApplicationException(Response.Status.NOT_FOUND);
       }
       catch(PatientServiceException ex) {
           throw new WebApplicationException(ex.getMessage());
       }
    }
    
    @GET
    @Produces(MediaType.APPLICATION_XML)
    public PatientRepresentation[] getPatientByNameDob(@QueryParam("name") String name, @QueryParam("dob") String dob) {
       Date birthDate = DatatypeConverter.parseDate(dob).getTime();
       PatientDTO[] patientDTOs = patientService.getPatientByNameDob(name, birthDate);
       PatientRepresentation[] patientReps = new PatientRepresentation[patientDTOs.length];
       for(int i = 0; i < patientDTOs.length; ++i) {
           patientReps[i] = new PatientRepresentation(patientDTOs[i], context);
       }
       return patientReps;
    }
    
    @POST
    @Consumes(MediaType.APPLICATION_XML)
    public Response addPatient(PatientRepresentation patientRep) {
        
        try {
            long id = patientService.createPatient(patientRep.getName(), patientRep.getDob(), 49, patientRep.getPatientId());
            UriBuilder ub = context.getAbsolutePathBuilder().path("{id}");
            URI url = ub.build(Long.toString(id));
            return Response.created(url).build();
        }
        catch (PatientServiceException ex) {
            throw new WebApplicationException(ex.getMessage());
        }
    }

}