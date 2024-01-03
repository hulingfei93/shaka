package edu.stevens.cs548.clinic.service.web.rest.resources;

import java.net.URI;
import java.util.List;

import javax.annotation.PostConstruct;
import javax.ejb.EJB;
import javax.ejb.Stateless;
import javax.enterprise.context.RequestScoped;
import javax.inject.Inject;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import javax.annotation.Resource;
import javax.transaction.SystemException;
import javax.transaction.UserTransaction;
import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.HeaderParam;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.FormParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.UriBuilder;
import javax.ws.rs.core.UriInfo;

import edu.stevens.cs548.clinic.domain.IPatientDAO;
import edu.stevens.cs548.clinic.domain.IPatientDAO.PatientException;
import edu.stevens.cs548.clinic.domain.IProviderDAO;
import edu.stevens.cs548.clinic.domain.IProviderDAO.ProviderException;
import edu.stevens.cs548.clinic.domain.ITreatmentDAO.TreatmentException;
import edu.stevens.cs548.clinic.domain.Patient;
import edu.stevens.cs548.clinic.domain.PatientDAO;
import edu.stevens.cs548.clinic.domain.Provider;
import edu.stevens.cs548.clinic.domain.ProviderDAO;
import edu.stevens.cs548.clinic.service.dto.ProviderDTO;
import edu.stevens.cs548.clinic.service.dto.TreatmentDTO;
import edu.stevens.cs548.clinic.service.ejb.IProviderService.PatientNotFoundException;
import edu.stevens.cs548.clinic.service.ejb.IProviderService.ProviderNotFoundException;
import edu.stevens.cs548.clinic.service.ejb.IProviderService.ProviderServiceException;
import edu.stevens.cs548.clinic.service.ejb.IProviderServiceRemote;
import edu.stevens.cs548.clinic.service.web.rest.ProviderRepresentation;
import edu.stevens.cs548.clinic.service.web.rest.TreatmentRepresentation;
import edu.stevens.cs548.clinic.service.web.rest.Treatment_PDO_to_Representation;
import edu.stevens.cs548.clinic.service.web.rest.data.DrugTreatmentType;
import edu.stevens.cs548.clinic.service.web.rest.data.RadiologyTreatmentType;
import edu.stevens.cs548.clinic.service.web.rest.data.SurgeryTreatmentType;

@Path("/provider")
@RequestScoped
public class ProviderResource {

    @Context
    private UriInfo context;
    private IPatientDAO patientDAO;
    private IProviderDAO providerDAO;
     
    public ProviderResource() {
    }
    
    @PersistenceContext(unitName="ClinicDomain")
    private EntityManager em;
    
    @Resource private UserTransaction utx; 
    
    @PostConstruct
    private void initialize() {
        patientDAO = new PatientDAO(em);
        providerDAO = new ProviderDAO(em);
    }
    
    @EJB
    private IProviderServiceRemote providerService;
    
    @POST
    @Consumes(MediaType.APPLICATION_XML)
    public Response addProvider(ProviderRepresentation providerRep) {
        try {
            long id = providerService.createProvider(providerRep.getName(), providerRep.getNPI());
            UriBuilder ub = context.getAbsolutePathBuilder().path("byNPI").queryParam("id", Long.toString(id));
            URI url = ub.build();
            return Response.created(url).build();
        }
        catch (ProviderServiceException ex) {
            throw new WebApplicationException(ex.getMessage());
        }
    }
    
    @POST
    @Path("addtreatment")
    @Consumes(MediaType.APPLICATION_XML)
    public Response addTreatment(TreatmentRepresentation treatmentRep, @QueryParam("pid") String patientDbIdStr, @QueryParam("npi") String NPIStr) {
        try {
            long patientDbId = Long.parseLong(patientDbIdStr);
            long NPI = Long.parseLong(NPIStr);
            long tid;
            
            Patient patient = patientDAO.getPatientByDbId(patientDbId);
            Provider provider = providerDAO.getProviderByNPI(NPI);
            if(patient == null || provider == null)
                throw new WebApplicationException(Response.Status.NOT_FOUND);
            
            utx.begin();
            if(treatmentRep.getDrugTreatment() != null) {
                DrugTreatmentType drug = treatmentRep.getDrugTreatment();
                tid = provider.addDrugTreatment(treatmentRep.getDiagnosis(), drug.getDrugName(), drug.getDosage(), patient);
            }
            else if(treatmentRep.getRadiologyTreatment() != null) {
                RadiologyTreatmentType radiology = treatmentRep.getRadiologyTreatment();
                tid = provider.addRadiologyTreatment(treatmentRep.getDiagnosis(), radiology.getDate(), patient);
            }
            else {
                SurgeryTreatmentType surgery = treatmentRep.getSurgeryTreatment();
                tid = provider.addSurgeryTreatment(treatmentRep.getDiagnosis(), surgery.getDate(), patient);
            }
            utx.commit();
            
            URI treatmentURI = context.getBaseUriBuilder().path("treatment").path("{id}").build(Long.toString(tid));
            return Response.created(treatmentURI).build();
            
        }
        catch(PatientException ex) {
            throw new WebApplicationException(Response.Status.NOT_FOUND);
        }
        catch(ProviderException ex) {
            throw new WebApplicationException(ex.getMessage());
        }
        catch(Exception ex) {
            throw new WebApplicationException(ex.getMessage());
        }
    }
    
    @GET
    @Path("byNPI")
    @Produces(MediaType.APPLICATION_XML)
    public ProviderRepresentation getProvider(@QueryParam("id") String NPI) {
       try {
           long npi = Long.parseLong(NPI);
           ProviderDTO providerDTO = providerService.getProviderByNPI(npi);
           ProviderRepresentation providerRep = new ProviderRepresentation(providerDTO, context);
           return providerRep;
       }
       catch(ProviderNotFoundException ex) {
           throw new WebApplicationException(Response.Status.NOT_FOUND);
       }
       catch(ProviderServiceException ex) {
           throw new WebApplicationException(ex.getMessage());
       }
    }
    
    @GET
    @Path("{npi}/treatments")
    @Produces(MediaType.APPLICATION_XML)
    public TreatmentRepresentation[] getTreatments(@PathParam("npi") String NPI, @HeaderParam("X-Patient") String patientDbId) {
        
       try {
            long npi = Long.parseLong(NPI);
            long id = Long.parseLong(patientDbId);
            Provider provider = providerDAO.getProviderByNPI(npi);
            Patient patient = patientDAO.getPatientByDbId(id);
            if(provider == null)
                throw new WebApplicationException(Response.Status.NOT_FOUND);
            List<Long> tids = provider.getTreatmentIds(patient);
            TreatmentRepresentation[] treatmentReps = new TreatmentRepresentation[tids.size()];
            for(int i = 0; i < tids.size(); ++i) {
                Treatment_PDO_to_Representation visitor = new Treatment_PDO_to_Representation(context);
                provider.visitTreatment(tids.get(i), visitor);
                treatmentReps[i] = visitor.getRep();
            }
            return treatmentReps;
        }
        catch(PatientException ex) {
             throw new WebApplicationException(ex.getMessage());
        }
        catch(ProviderException ex) {
             throw new WebApplicationException(ex.getMessage());
        }
        catch(TreatmentException ex) {
             throw new WebApplicationException(ex.getMessage());
        }
    }
}
